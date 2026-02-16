#ifndef TOKENS_H
#define TOKENS_H
#include <stddef.h>

#define EQLEN 100 // Maximum length of equation
#define NUMLEN 98 // Maximum length of a number
// List of defined functions
extern const char *(FUNCLIST[]);

typedef enum {
    NUMERICAL,
    VARIABLE, 
    OPERATION,
    FUNCTION,
    LPAREN,
    RPAREN,
    END
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct {
    char *equation;
    size_t pos;
    Token current;
} Interpreter;

#endif