#ifndef ERR_H
#define ERR_H
#include "TOKENS.h"

typedef enum {
    MULTIPLE_DECIMALS,
    MISSING_OPERAND,
    INVALID_OPERATOR_COMBINATION,
    UNKNOWN_SYMBOL,
    DIV_BY_ZERO,
    MISSING_PARENTHESES,
    INVALID_ARGUMENT,
    INIT_ISSUES
} ErrType;

void errFunc(ErrType, Token *);

#endif