#include "ExprTree.h"
#include "Token.h"
#include "Utils.h"

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
