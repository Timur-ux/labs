#ifndef EXPR_TREE_H_
#define EXPR_TREE_H_
#include "CharStream.h"
#include "StackMacros.h"
#include "Token.h"

STACK(Tokens, TokenVoid);
Tokens DijkstraToPrefix(CharStream *stream);

typedef struct ExprTree ExprTree;

ExprTree *ExprTree_Create(Tokens *tokens);
void ExprTree_Destroy(ExprTree * tree);

void ExprTree_Print(ExprTree* tree, FILE *file);
void ExprTree_Differentiate(ExprTree *tree, char var);
#endif // !EXPR_TREE_H_
