#include <stdio.h>
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
    Interpreter test = {.pos = 0};
    char input[100];
    while (1) {
        fgets(input, 100, stdin);
        test.equation = input; 

        printf("%lf\n", parse(&test));
        test.pos = 0;
    }
}