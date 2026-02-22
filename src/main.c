#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TOKENS.h"
#include "TOKENISE.h"
#include "PARSE.h"
#include "ERR.h"

int main(int argc, char **argv) {
    Interpreter eq = {.pos = 0};
    char input[EQLEN];
    int var_calc = 0;       // 1 if the calculator is set to variable mode
    double var = 0;

    if (argc == 2) {
        var_calc = 1;
    } else if (argc > 2) {
        errFunc(INIT_ISSUES, NULL);
    }
    
    if (var_calc) {
        printf("Enter an expresion:\n");
        fgets(input, EQLEN, stdin);
        eq.equation = input; 

        while (1) {
            printf("Evaluate at:\n");
            if (!scanf(" %lf", &var)) {
                errFunc(INVALID_ARGUMENT, NULL);
                exit(1);
            }

            printf("%lf\n", parse(&eq, var));
            free((eq.current).value);
        }
    } else {
        while (1) {
            printf("Enter an expresion:\n");
            fgets(input, EQLEN, stdin);
            eq.equation = input;

            printf("%lf\n", parse(&eq, INFINITY));
            free((eq.current).value);
        }
    }

    
}