#include <stdlib.h>
#include <stdio.h>
#include "ERR.h"
#include "TOKENS.h"

void errFunc(ErrType err_type, Token *return_token) {
    if (return_token) {free(return_token->value);}

    switch (err_type) {
    case MULTIPLE_DECIMALS:
        printf("You've put too many decimal points in a number.\n");
        break;
    case MISSING_OPERAND:
        printf("Seems you've forgotten something at the end.\n");
        break;
    case INVALID_OPERATOR_COMBINATION:
        printf("No clue what you mean, bud.\n");
        break;
    case UNKNOWN_SYMBOL:
        printf("You've entered a symbol I don't recognise.\n");
        break;
    case DIV_BY_ZERO:
        printf("You've tried to divide by zero.!\n");
        break;
    case MISSING_PARENTHESES:
        printf("You've forgotten a parenthesis.\n");
        break;
    case INVALID_ARGUMENT:
        printf("You've entered an invalid argument for a function.\n");
        break;
    default:
        printf("omfg what did you do?!\n");
        break;
    }

    exit(1);
}