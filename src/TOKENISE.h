#ifndef TOKENISE_H
#define TOKENISE_H
#include "TOKENS.h"

static void advanceChar(Interpreter *, char *);

Token tokenise(Interpreter *);

#endif