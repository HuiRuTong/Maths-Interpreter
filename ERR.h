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
} ErrType;

void errFunc(ErrType, char *, Token *);

#endif