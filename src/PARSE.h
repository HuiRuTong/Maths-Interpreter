#ifndef PARSE_H
#define PARSE_H
#include "TOKENS.h"

typedef enum {
    SQRT,
    LOG,
    ATRIG,
    ACOSH,
    ATANH
} FuncType;

extern int inFunction;

static void advanceToken(Interpreter *);

static int isValid(FuncType, double arg);

double term(Interpreter *interpreter, TokenType);
double func(Interpreter *interpreter, TokenType);
double exponent(Interpreter *interpreter, TokenType);
double mulDiv(Interpreter *interpreter, TokenType);
double addSub(Interpreter *interpreter, TokenType);

double parse(Interpreter *interpreter);

#endif