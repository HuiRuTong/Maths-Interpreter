#include <stdio.h>
#include <stdlib.h>

void errFunc(void (*errType)(), char *curr_char, char *return_token) {
    if (!curr_char) {free(curr_char);}
    if (!return_token) {free(return_token);}
    (*errType)();
    exit(1);
}

void errMultipleDecimals() {
    printf("You've put too many decimal points in a number.\n");
}
void errMissingOperand() {
    printf("Seems you've forgotten something at the end.\n");
}
void errInvalidOperatorCombination() {
    printf("No clue what you mean, bud.\n");
}
void errUnknownSymbol() {
    printf("You've entered a symbol I don't recognise.\n");
}
void errUnknownFunction () {
    printf("You've entered a function I don't recognise.\n");
}
void errDivByZero() {
    printf("Absolutely not!");
}