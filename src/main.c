#include <stdio.h>
#include <stdlib.h>
#include "TOKENS.h"
#include "TOKENISE.h"
#include "PARSE.h"
#include "ERR.h"

/*
    To implement:
    1. a mode for you to enter up to 2 variables
    2. basic calculator mode <- current
*/
int main() {
    Interpreter eq = {.pos = 0};
    char input[EQLEN];
    while (1) {
        fgets(input, 100, stdin);
        eq.equation = input; 

        printf("%lf\n", parse(&eq));
        free((eq.current).value);
    }
}