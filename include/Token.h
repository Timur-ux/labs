#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

#include "CharStream.h"
typedef enum { TOKEN_NONE, TOKEN_OP, TOKEN_NUMBER, TOKEN_VAR } TokenType;

// enum equivalent to:
// int TOKEN_NONE = 0;
// int TOKEN_OP = 1;
// int TOKEN_NUMBER = 2;
// int TOKEN_VAR = 3;

typedef enum {
	OP_NONE,
  OP_PLUS,
  OP_MINUS_BINARY,
  OP_MINUS_UNARY,
  OP_MULT,
  OP_DIVIDE,
  OP_POW,
  OP_BRACKET_OPEN,
  OP_BRACKET_CLOSE
} OpType;

// Variant 1
typedef struct {
  TokenType type;
  union {
    OpType op;
    char var;
    double number;
  } data;
} Token;
// Token token;
// token.data.number
// token.data.var
// token.data.op

// Variant 2
typedef struct {
  TokenType type;
} TokenBase;

typedef struct {
  TokenType type;
  double value;
} TokenNumber;

typedef struct {
  TokenType type;
  char value;
} TokenVariable;

typedef struct {
  TokenType type;
  OpType value;
} TokenOp;

// Variant 3
typedef struct {
  TokenType type;
  void *data;
} TokenVoid;

TokenVoid Token_Read(CharStream *stream);
void Token_Print(TokenVoid token, FILE *file);
void Token_Destroy(TokenVoid *token);

OpType Token_GetOp(TokenVoid token);
double Token_GetNumber(TokenVoid token);
char Token_GetVar(TokenVoid token);

bool Token_Equal(TokenVoid lhs, TokenVoid rhs);

#endif // TOKEN_H
