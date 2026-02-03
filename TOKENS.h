#ifndef TOKENS_H
#define TOKENS_H
#include <stdlib.h>

typedef enum {
    NUMERICAL,
    OPERATION,
    FUNCTION,
    BRACKETS,
    SPACE,
    END
} TokenTypes;

typedef struct {
    TokenTypes type;
    char *value;
} Token;

typedef struct {
    char *equation;
    size_t pos;
    Token current;
} Interpreter;

#endif