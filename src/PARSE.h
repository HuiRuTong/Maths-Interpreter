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

static void advanceToken(Interpreter *);

static int isValid(FuncType, double arg);

double term(Interpreter *interpreter);
double func(Interpreter *interpreter);
double exponent(Interpreter *interpreter);
double mulDiv(Interpreter *interpreter);
double addSub(Interpreter *interpreter);
double parse(Interpreter *interpreter);

#endif