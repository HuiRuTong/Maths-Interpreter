#include <stdlib.h>
#include <stdio.h>
#include "ERR.h"
#include "TOKENS.h"

void errFunc(ErrType err_type, char *curr_char, Token *return_token) {
    if (!curr_char) {free(curr_char);}
    if (!return_token->value) {free(return_token->value);}

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
    case UNKNOWN_FUNCTION:
        printf("You've entered a function I don't recognise.\n");
        break;
    case DIV_BY_ZERO:
        printf("Absolutely not!\n");
        break;
    default:
        printf("omfg what did you do?!\n");
        break;
    }

    exit(1);
}