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

double term(Interpreter *, TokenType, double);
double exponent(Interpreter *, TokenType, double);
double mulDiv(Interpreter *, TokenType, double);
double addSub(Interpreter *, TokenType, double);

double parse(Interpreter *, double);

#endif