#ifndef EXPR_TREE_H_
#define EXPR_TREE_H_
#include "CharStream.h"
#include "StackMacros.h"
#include "Token.h"

STACK(Tokens, TokenVoid);
Tokens DijkstraToPrefix(CharStream *stream);
#endif // !EXPR_TREE_H_
