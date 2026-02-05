#ifndef PARSE_H
#define PARSE_H
#include "TOKENS.h"

static void advanceToken(Interpreter *);

double term(Interpreter *interpreter);
double exponent(Interpreter *interpreter);
double mulDiv(Interpreter *interpreter);
double addSub(Interpreter *interpreter);
double parse(Interpreter *interpreter);

#endif