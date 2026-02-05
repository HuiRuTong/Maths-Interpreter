#ifndef ERR_H
#define ERR_H
#include "TOKENS.h"

typedef enum {
    MULTIPLE_DECIMALS,
    MISSING_OPERAND,
    INVALID_OPERATOR_COMBINATION,
    UNKNOWN_SYMBOL,
    UNKNOWN_FUNCTION,
    DIV_BY_ZERO,
    MISSING_PARENTHESES
} ErrType;

void errFunc(ErrType, Token *);

#endif