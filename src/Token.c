#include "Token.h"
#include "CharStream.h"
#include "Utils.h"
#include "sys/types.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static OpType opTypes[] = {OP_NONE,        OP_PLUS,         OP_MINUS_BINARY,
                           OP_MINUS_UNARY, OP_MULT,         OP_DIVIDE,
                           OP_POW,         OP_BRACKET_OPEN, OP_BRACKET_CLOSE};
static char opLetters[] = {'?', '+', '-', '-', '*', '/', '^', '(', ')'};

void Token_Print(TokenVoid token, FILE *file) {
  if (token.type == TOKEN_NONE)
    fprintf(file, "<none>");
  else if (token.type == TOKEN_NUMBER)
    fprintf(file, "%lf", Token_GetNumber(token));
  else if (token.type == TOKEN_VAR)
    fprintf(file, "%c", Token_GetVar(token));
  else if (token.type == TOKEN_OP)
    fprintf(file, "%c", opLetters[Token_GetOp(token)]);
  else
    FatalError("Undefined token type!");
}

TokenVoid Token_Op(OpType op) {
  TokenVoid token;
  token.type = TOKEN_OP;
  token.data = &opTypes[op];
  return token;
}

TokenVoid Token_Number(double number) {
  TokenVoid token;
  token.type = TOKEN_NUMBER;
  token.data = malloc(sizeof(double));
  *(double *)token.data = number;
  return token;
}

TokenVoid Token_Var(char var) {
  TokenVoid token;
  token.type = TOKEN_VAR;
  token.data = malloc(sizeof(char));
  *(char *)token.data = var;
  return token;
}

TokenVoid Token_None() {
  TokenVoid token;
  token.type = TOKEN_NONE;
  return token;
}

OpType GetOp(char c, TokenVoid prevToken) {
  switch (c) {
  case '+':
    return OP_PLUS;
  case '-':
    if (prevToken.type == TOKEN_NONE ||
        (prevToken.type == TOKEN_OP &&
         Token_GetOp(prevToken) == OP_BRACKET_OPEN))
      return OP_MINUS_UNARY;
    else
      return OP_MINUS_BINARY;
  case '*':
    return OP_MULT;
  case '/':
    return OP_DIVIDE;
  case '(':
    return OP_BRACKET_OPEN;
  case ')':
    return OP_BRACKET_CLOSE;
  case '^':
    return OP_POW;
  default:
    return OP_NONE;
  }
}

TokenVoid Token_Read(CharStream *stream) {
  static bool prevInitialized = false;
  static TokenVoid prevToken;
  if (!prevInitialized) {
    prevToken = Token_None();
    prevInitialized = true;
  }

  while (isspace(CharStream_Get(stream)))
    CharStream_Next(stream);

  TokenVoid token;
  int c = CharStream_Get(stream);
  if (isalpha(c))
    return CharStream_Next(stream), prevToken = Token_Var(c);
  if (isdigit(c) || c == '.') {
    double value = 0, multiplicator = 1;
    bool wasDot = false;
    while (isdigit(c) || c == '.') {
      if (isdigit(c)) {
        if (!wasDot)
          value = value * 10 + (c - '0');
        else
          multiplicator /= 10, value += (c - '0') * multiplicator;
      } else {
        if (!wasDot)
          wasDot = true;
        else
          FatalError("Multiple dots in number not allowed!");
      }
      CharStream_Next(stream);
      c = CharStream_Get(stream);
    }
    return prevToken = Token_Number(value);
  }
  OpType op = GetOp(c, prevToken);
  CharStream_Next(stream);
  if (op != OP_NONE)
    return prevToken = Token_Op(op);
  return prevToken = Token_None();
}

void Token_Destroy(TokenVoid *token) {
  switch (token->type) {
  case TOKEN_VAR:
  case TOKEN_NUMBER:
    free(token->data);
    break;
  default:
    break;
  }

  token->type = TOKEN_NONE;
}

OpType Token_GetOp(TokenVoid token) {
  if (token.type != TOKEN_OP)
    FatalError("Type mismatch!");
  return *(OpType *)token.data;
}

double Token_GetNumber(TokenVoid token) {
  if (token.type != TOKEN_NUMBER)
    FatalError("Type mismatch!");
  return *(double *)token.data;
}

char Token_GetVar(TokenVoid token) {
  if (token.type != TOKEN_VAR)
    FatalError("Type mismatch!");
  return *(char *)token.data;
}

bool Token_Equal(TokenVoid lhs, TokenVoid rhs) {
  if (lhs.type != rhs.type)
    return false;

  switch (lhs.type) {
  case TOKEN_NUMBER:
    return *(double *)lhs.data == *(double *)rhs.data;
  case TOKEN_VAR:
    return *(char *)lhs.data == *(char *)rhs.data;
  case TOKEN_OP:
    return *(OpType *)lhs.data == *(OpType *)rhs.data;
  case TOKEN_NONE:
    return true;
  }

  return false;
}

TokenVoid Token_Copy(TokenVoid token) {
  switch (token.type) {
  case TOKEN_NUMBER:
    return Token_Number(Token_GetNumber(token));
  case TOKEN_VAR:
    return Token_Var(Token_GetVar(token));
  case TOKEN_OP:
    return Token_Op(Token_GetOp(token));
  default:
    return Token_None();
  }
}
