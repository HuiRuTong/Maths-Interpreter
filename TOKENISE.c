#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "TOKENS.h"
#include "ERR.h"

const char *(FUNCLIST[]) = {"sqrt", "cbrt", "ln", "log10", "ceil", "floor",
                            "sin", "cos", "tan", "asin", "acos", "atan"
                            "sinh", "cosh", "tanh", "asinh", "acosh", "atanh"};

static void advanceChar(Interpreter *interpreter, char *c) {
    (interpreter->pos)++;
    *c = (interpreter->equation)[interpreter->pos];
}

Token tokenise(Interpreter *interpreter) {
    /* 
        Reads and tokenises character at pos
    */
    char *current_char = malloc(NUMLEN * sizeof(char));
    current_char[0] = (interpreter->equation)[interpreter->pos];
    current_char[1] = '\0';
    Token return_token = {.value = malloc(NUMLEN * sizeof(char))};
    
    while ((interpreter->equation)[interpreter->pos] == ' ') {
        advanceChar(interpreter, current_char);
    }
    strcpy(return_token.value, current_char);

    if (interpreter->pos == strlen(interpreter->equation) - 1) {
        return_token.type = END;
        return_token.value = "\n";
        free(current_char);
        return return_token;
    }

    if (isdigit(current_char[0]) || current_char[0] == '.') {
        return_token.type = NUMERICAL;
        int decimals = 0;

        if (current_char[0] == '.') {
            decimals += 1;
        }

        // Appends other digits onto the first if there's > 1 digit
        while (isdigit((interpreter->equation)[(interpreter->pos) + 1]) ||
              (interpreter->equation)[(interpreter->pos) + 1] == '.' ||
              (interpreter->equation)[(interpreter->pos) + 1] == ' ') {
            advanceChar(interpreter, current_char);

            if (current_char[0] == ' ') {
                continue;
            } 

            if (current_char[0] == '.' && ++decimals > 1) {errFunc(MULTIPLE_DECIMALS, current_char, &return_token);}
            strcat(return_token.value, current_char);
        }
        free(current_char);
        return return_token;
    }

    if (ispunct(current_char[0])) {
        return_token.type = OPERATION;
        int sign = 0;
        int muldiv = 0;
        int exp = 0;

        do {
            if (current_char[0] != ' ') {
                // If there's nothing after an operator
                if (current_char[0] == '\n') {errFunc(MISSING_OPERAND, current_char, &return_token);}

                // To handle strings of + and -
                if (current_char[0] == '+' || current_char[0] == '-') {
                    // If there was a * or / beforehand, reject the equation; ^- is OK tho
                    if (muldiv) {errFunc(INVALID_OPERATOR_COMBINATION, current_char, &return_token);}

                    sign = 1;
                    muldiv = 0;
                    exp = 0;
                    if (current_char[0] == '-') {
                        sign *= -1;
                    }
                } else if (current_char[0] == '*' || current_char[0] == '/') {
                    // If there was a + or - beforehand, reject the equation
                    if (sign || exp) {errFunc(INVALID_OPERATOR_COMBINATION, current_char, &return_token);}

                    sign = 0;
                    exp = 0;

                    if (++muldiv > 1) {errFunc(INVALID_OPERATOR_COMBINATION, current_char, &return_token);}
                } else if (current_char[0] == '^') {
                    if (sign || muldiv) {errFunc(INVALID_OPERATOR_COMBINATION, current_char, &return_token);}

                    muldiv = 0;

                    if (++exp > 1) {errFunc(INVALID_OPERATOR_COMBINATION, current_char, &return_token);}
                } else {errFunc(UNKNOWN_SYMBOL, current_char, &return_token);}
            }

            advanceChar(interpreter, current_char);
        } while (ispunct(current_char[0]) || isspace(current_char[0]));
        (interpreter->pos)--;
        
        if (sign > 0) {
            return_token.value = "+";
        } else if (sign < 0) {
            return_token.value = "-";
        }

        free(current_char);
        return return_token;
    }

    // Not used yet
    if (isalpha(current_char[0])) {
        return_token.type = FUNCTION;
        char fx[6];
        int i = 0;
        fx[0] = '\0';

        do {
            if (i > 4) {errFunc(UNKNOWN_FUNCTION, current_char, &return_token);}

            fx[i] = current_char[0];
            fx[i+1] = '\0';

            advanceChar(interpreter, current_char);
        } while (isalpha(current_char[0]));
        (interpreter->pos)--; // Set the position back to the last character of the funciton

        if (strlen(fx) < 3) {errFunc(UNKNOWN_FUNCTION, current_char, &return_token);}

        for (int i = 0; i < 18; i++) {
            if (!strcmp(fx, FUNCLIST[i])) {
                strcpy(return_token.value, fx);
                free(current_char);
                return return_token;
            }
        }
        errFunc(UNKNOWN_FUNCTION, current_char, &return_token);
    }

    if (!strcmp(current_char, "(") || (!strcmp(current_char, ")"))) {
        return_token.type = BRACKETS;
        free(current_char);
        return return_token;
    }

    if(current_char[0] == ' ') {
        return_token.type = SPACE;
        free(current_char);
        return return_token;
    }

    errFunc(UNKNOWN_SYMBOL, current_char, &return_token);
}