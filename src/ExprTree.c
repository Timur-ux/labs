#include "ExprTree.h"
#include "Token.h"
#include "Utils.h"
#include <stdlib.h>

void DijkstraToPrefix_impl(CharStream *stream, Tokens *result, Tokens *temp);
Tokens DijkstraToPrefix(CharStream *stream) {
  Tokens result = Tokens_Create(), temp = Tokens_Create();
  DijkstraToPrefix_impl(stream, &result, &temp);
  Tokens_Destroy(&temp);
  return result;
}

size_t GetOpPriority(OpType op) {
  switch (op) {
  case OP_BRACKET_OPEN:
  case OP_BRACKET_CLOSE:
    return 0;
  case OP_PLUS:
  case OP_MINUS_BINARY:
    return 1;
  case OP_MULT:
  case OP_DIVIDE:
    return 2;
  case OP_POW:
    return 3;
  case OP_MINUS_UNARY:
    return 4;
  default:
    FatalError("Undefined operation!");
  }
  return 0;
}

typedef enum { ASSOC_LEFT, ASSOC_RIGHT } AssocType;

AssocType GetOpAssoc(OpType op) {
  if (op == OP_POW)
    return ASSOC_RIGHT;
  return ASSOC_LEFT;
}

bool IsReplacementNeed(TokenVoid *currentToken, TokenVoid *stackToken) {
  size_t currentPriority = GetOpPriority(Token_GetOp(*currentToken));
  size_t stackPriority = GetOpPriority(Token_GetOp(*stackToken));

  if (GetOpAssoc(Token_GetOp(*currentToken)) == ASSOC_LEFT)
    return currentPriority <= stackPriority;
  return currentPriority < stackPriority;
}

void DijkstraToPrefix_impl(CharStream *stream, Tokens *result, Tokens *temp) {
  TokenVoid token, tempToken;
  OpType op;
  while ((token = Token_Read(stream)).type != TOKEN_NONE) {
    switch (token.type) {
    case TOKEN_NUMBER:
    case TOKEN_VAR:
      Tokens_Push(result, token);
      break;
    case TOKEN_OP:
      op = Token_GetOp(token);
      if (op == OP_BRACKET_OPEN) {
        Tokens_Push(temp, token);
        continue;
      }
      if (op == OP_BRACKET_CLOSE) {
        while (!Tokens_Empty(temp) &&
               ((tempToken = *Tokens_Top(temp)).type != TOKEN_OP ||
                Token_GetOp(tempToken) != OP_BRACKET_OPEN))
          Tokens_Push(result, *Tokens_Top(temp)), Tokens_Pop(temp);

        if (Tokens_Empty(temp))
          FatalError("For ) paired ( not found!");
        Tokens_Pop(temp);
        continue;
      }
      while (!Tokens_Empty(temp) && IsReplacementNeed(&token, Tokens_Top(temp)))
        Tokens_Push(result, *Tokens_Top(temp)), Tokens_Pop(temp);

      Tokens_Push(temp, token);
      break;
    }
  }
  while (!Tokens_Empty(temp))
    Tokens_Push(result, *Tokens_Top(temp)), Tokens_Pop(temp);
}

typedef struct Node {
  TokenVoid token;
  struct Node *left, *right;
} Node;

struct ExprTree {
  Node *root;
};

Node *Node_Create(Tokens *tokens) {
  if (Tokens_Empty(tokens))
    FatalError("Expected non empty stack");

  Node *node = (Node *)malloc(sizeof(Node));
  node->left = node->right = NULL;
  TokenVoid token = *Tokens_Top(tokens);
  Tokens_Pop(tokens);
  node->token = token;
  if (token.type == TOKEN_NUMBER || token.type == TOKEN_VAR)
    return node;

  node->right = Node_Create(tokens);
  if (Token_GetOp(token) != OP_MINUS_UNARY)
    node->left = Node_Create(tokens);

  return node;
}

ExprTree *ExprTree_Create(Tokens *tokens) {
  ExprTree *tree = (ExprTree *)malloc(sizeof(ExprTree));
  tree->root = Node_Create(tokens);
  return tree;
}

void Node_Destroy(Node *node) {
  if (!node)
    return;
  Node_Destroy(node->left);
  Node_Destroy(node->right);

  Token_Destroy(&node->token);
  free(node);
}
void ExprTree_Destroy(ExprTree *tree) {
  Node_Destroy(tree->root);
  free(tree);
}

bool IsBracketsWrapNeed(TokenVoid parent, TokenVoid child) {
  return parent.type == TOKEN_OP && child.type == TOKEN_OP &&
         GetOpPriority(Token_GetOp(parent)) > GetOpPriority(Token_GetOp(child));
}
void Node_Print(Node *node, FILE *file) {
  if (!node)
    return;

  if (node->left && IsBracketsWrapNeed(node->token, node->left->token)) {
    fprintf(file, "(");
    Node_Print(node->left, file);
    fprintf(file, ")");
  } else
    Node_Print(node->left, file);

  fprintf(file, " ");
  Token_Print(node->token, file);
  fprintf(file, " ");

  if (node->right && IsBracketsWrapNeed(node->token, node->right->token)) {
    fprintf(file, "(");
    Node_Print(node->right, file);
    fprintf(file, ")");
  } else
    Node_Print(node->right, file);
}

void ExprTree_Print(ExprTree *tree, FILE *file) {
  Node_Print(tree->root, file);
}

void Node_PrintAsTree(Node *node, FILE *file, int depth) {
  if (!node)
    return;

  Node_PrintAsTree(node->right, file, depth + 1);

  for (int i = 0; i < depth; ++i)
    printf("\t");
  Token_Print(node->token, file);
  printf("\n");

  Node_PrintAsTree(node->left, file, depth + 1);
}

void ExprTree_PrintAsTree(ExprTree *tree, FILE *file) {
  Node_PrintAsTree(tree->root, file, 0);
}

Node *Node_Copy(Node *target) {
  if (!target)
    return NULL;

  Node *node = (Node *)malloc(sizeof(Node));
  node->token = Token_Copy(target->token);
  node->left = Node_Copy(target->left);
  node->right = Node_Copy(target->right);
  return node;
}

Node *Node_Create3(TokenVoid token, Node *left, Node *right) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->token = token;
  node->left = left;
  node->right = right;
  return node;
}

Node *Node_Diff(Node *node, char var) {
  if (!node)
    return NULL;
  TokenVoid token = node->token;
  if (token.type == TOKEN_NUMBER) {
    node->token = Token_Number(0);
    Token_Destroy(&token);
    return node;
  }
  if (token.type == TOKEN_VAR) {
    double val = 0;
    if (Token_GetVar(token) == var)
      val = 1;
    node->token = Token_Number(val);
    Token_Destroy(&token);
    return node;
  }

  OpType op = Token_GetOp(token);
  if (op == OP_PLUS || op == OP_MINUS_BINARY || op == OP_MINUS_UNARY) {
    node->left = Node_Diff(node->left, var);
    node->right = Node_Diff(node->right, var);
  }
  if (op == OP_MULT) {
    // (u*v)' == u' * v + u * v'
    Node *u = node->left, *v = node->right;
    Node *du = Node_Diff(Node_Copy(u), var);
    Node *dv = Node_Diff(Node_Copy(v), var);

    node->token = Token_Op(OP_PLUS);
    Token_Destroy(&token);
    node->left = Node_Create3(Token_Op(OP_MULT), du, v);
    node->right = Node_Create3(Token_Op(OP_MULT), u, dv);
    return node;
  }
  if (op == OP_DIVIDE) {
    // (u / v)' = (u' * v - u * v') / (v * v)
    Node *u = node->left, *v = node->right;
    Node *du = Node_Diff(Node_Copy(u), var);
    Node *dv = Node_Diff(Node_Copy(v), var);
    node->left = Node_Create3(Token_Op(OP_MINUS_BINARY),
                              Node_Create3(Token_Op(OP_MULT), du, v),
                              Node_Create3(Token_Op(OP_MULT), u, dv));
    node->right = Node_Create3(Token_Op(OP_DIVIDE), Node_Copy(v), Node_Copy(v));
    return node;
  }
  if (op == OP_POW) {
    TokenVoid lToken = node->left->token;
    if (lToken.type == TOKEN_VAR && Token_GetVar(lToken) == var) {
      Node *u = node->left, *alpha = node->right;
      Node *nMinus = Node_Create3(Token_Op(OP_MINUS_BINARY), Node_Copy(alpha),
                                  Node_Create3(Token_Number(1), NULL, NULL));
      node->right = nMinus;
      Node *result = Node_Create3(Token_Op(OP_MULT), alpha, node);
      return result;
    }
    if (lToken.type != TOKEN_OP) {
      Node *f_u = node->right, *d_f_u = Node_Diff(Node_Copy(f_u), var);
      Node *alpha = node->left;
      Node *alphaMinus1 =
          Node_Create3(Token_Op(OP_MINUS_BINARY), Node_Copy(alpha),
                       Node_Create3(Token_Number(1), NULL, NULL));
      Node *lnAlpha = Node_Create3(
          Token_Op(OP_MINUS_BINARY), alphaMinus1,
          Node_Create3(Token_Op(OP_DIVIDE),
                       Node_Create3(Token_Op(OP_POW), Node_Copy(alphaMinus1),
                                    Node_Create3(Token_Number(2), NULL, NULL)),
                       Node_Create3(Token_Number(2), NULL, NULL)));

      Node *result =
          Node_Create3(Token_Op(OP_MULT), node,
                       Node_Create3(Token_Op(OP_MULT), lnAlpha, d_f_u));
      return result;
    }
  }
  return node;
}

void ExprTree_Differentiate(ExprTree *tree, char var) {
  tree->root = Node_Diff(tree->root, var);
}
