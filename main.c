#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define EQLEN 100
#define NUMLEN 98
const char *(FUNCLIST[]) = {"sqrt", "cbrt", "ln", "log10", "ceil", "floor",
                            "sin", "cos", "tan", "asin", "acos", "atan"
                            "sinh", "cosh", "tanh", "asinh", "acosh", "atanh"};

#include "TOKENS.h"
#include "ERR.h"

char advanceChar(Interpreter *);
void advanceToken(Interpreter *);

Token tokenise(Interpreter *interpreter) {
    /* 
        Reads and tokenises character at pos
    */
    char *current_char = malloc(NUMLEN * sizeof(char));
    Token return_token = {.value = malloc(NUMLEN * sizeof(char))};
    /* uhhhh im too tired rn
    
    while ((interpreter->equation)[interpreter->pos] == ' ') {
        advanceChar(interpreter);
    }*/

    if (interpreter->pos == strlen(interpreter->equation) - 1) {
        return_token.type = END;
        return_token.value = "\n";
        free(current_char);
        return return_token;
    }

    current_char[0] = (interpreter->equation)[interpreter->pos];
    current_char[1] = '\0';
    strcpy(return_token.value, current_char);

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
            current_char[0] = advanceChar(interpreter);

            if (current_char[0] == ' ') {
                continue;
            } 

            if (current_char[0] == '.' && ++decimals > 1) {errFunc(errMultipleDecimals, current_char, return_token.value);}
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
                if (current_char[0] == '\n') {errFunc(errMissingOperand, current_char, return_token.value);}

                // To handle strings of + and -
                if (current_char[0] == '+' || current_char[0] == '-') {
                    // If there was a * or / beforehand, reject the equation; ^- is OK tho
                    if (muldiv) {errFunc(errInvalidOperatorCombination, current_char, return_token.value);}

                    sign = 1;
                    muldiv = 0;
                    exp = 0;
                    if (current_char[0] == '-') {
                        sign *= -1;
                    }
                } else if (current_char[0] == '*' || current_char[0] == '/') {
                    // If there was a + or - beforehand, reject the equation
                    if (sign || exp) {errFunc(errInvalidOperatorCombination, current_char, return_token.value);}

                    sign = 0;
                    exp = 0;

                    if (++muldiv > 1) {errFunc(errInvalidOperatorCombination, current_char, return_token.value);}
                } else if (current_char[0] == '^') {
                    if (sign || muldiv) {
                        errFunc(errInvalidOperatorCombination, current_char, return_token.value);
                    }

                    muldiv = 0;

                    if (++exp > 1) {
                        errFunc(errInvalidOperatorCombination, current_char, return_token.value);
                    }
                } else {
                    errFunc(errUnknownSymbol, current_char, return_token.value);
                }
            }
            current_char[0] = advanceChar(interpreter);
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

    if (isalpha(current_char[0])) {
        return_token.type = FUNCTION;
        char fx[6];
        int i = 0;
        fx[0] = '\0';

        do {
            if (i > 4) {
                errFunc(errUnknownFunction, current_char, return_token.value);
            }

            fx[i] = current_char[0];
            fx[i+1] = '\0';

            current_char[0] = advanceChar(interpreter);
        } while (isalpha(current_char[0]));
        (interpreter->pos)--; // Set the position back to the last character of the funciton

        if (strlen(fx) < 3) {
            errFunc(errUnknownFunction, current_char, return_token.value);
        }

        for (int i = 0; i < 18; i++) {
            if (!strcmp(fx, FUNCLIST[i])) {
                strcpy(return_token.value, fx);
                free(current_char);
                return return_token;
            }
        }
        errUnknownFunction();
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

    errUnknownSymbol();
}

char advanceChar(Interpreter *interpreter) {
    (interpreter->pos)++;
    return (interpreter->equation)[interpreter->pos];
}

void advanceToken(Interpreter *interpreter) {
    (interpreter->pos)++;
    (interpreter->current) = tokenise(interpreter);
}

double exponent(Interpreter *interpreter) {
    double result;
    if (interpreter->pos == 0 && (interpreter->current).type == OPERATION) {
        if (!strcmp((interpreter->current).value, "+") || !strcmp((interpreter->current).value, "-")) {
            result = 0; // To handle cases like +5-8 and -5*4
        } else {
            errFunc(errInvalidOperatorCombination, NULL, NULL);
        }
    } else {
        result = atof((interpreter->current).value);
        advanceToken(interpreter);
    }
    
    while (!strcmp((interpreter->current).value, "^") || !strcmp((interpreter->current).value, " ")) {
        if (strcmp((interpreter->current).value, " ")) {
            advanceToken(interpreter);
            result = pow(result, atof((interpreter->current).value));
        }

        if ((interpreter->current).type == END) {
            break;
        }
        advanceToken(interpreter);
    }
    
    return result;
}

double mulDiv(Interpreter *interpreter) {
    double result = exponent(interpreter);

    // Evaluate strings of * and / e.g. 8 * 2 / 4 * 5
    while (!strcmp((interpreter->current).value, "*") || !strcmp((interpreter->current).value, "/") || !strcmp((interpreter->current).value, " ")) {
        if (strcmp((interpreter->current).value, " ")) {
            if (!strcmp((interpreter->current).value, "*")) {
                advanceToken(interpreter);
                result *= exponent(interpreter);
            } else {
                advanceToken(interpreter);
                result /= exponent(interpreter);
            }
        }

        if ((interpreter->current).type == END) {
            break;
        }
    }

    return result;
}

double addSub(Interpreter *interpreter) {
    double result = mulDiv(interpreter);

    while (!strcmp((interpreter->current).value, "+") || !strcmp((interpreter->current).value, "-") || !strcmp((interpreter->current).value, " ")) {
        if (strcmp((interpreter->current).value, " ")) {
            if (!strcmp((interpreter->current).value, "+")) {
                advanceToken(interpreter);
                result += mulDiv(interpreter);
            } else {
                advanceToken(interpreter);
                result -= mulDiv(interpreter);
            }
        }

        if ((interpreter->current).type == END) {
            break;
        }
    }

    return result;
}

double parse(Interpreter *interpreter) {
    interpreter->current = tokenise(interpreter);
    double result = addSub(interpreter);

    return result;
}

// DEAL WITH LEAKED MEM!!! //

char *removeSpaces(char *);

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

// Returns a str with its space removed
/*
    In lieu of the realisation that doing this involves an unnecessary traversal of
    the equation, I have rewritten the tokeniser to ignore spaces instead.
    This is kept in case anything goes wrong.
*/
char *removeSpaces(char *str) {
    char *new = malloc(strlen(str) * sizeof(char));
    int i = 0;
    while (*str != '\0') { // NULL doesn't work because there's still unused allocated space
        if (*str != ' ') {
            *(new+i) = *str;
            i++;
        }
        str++;
    }
    return new;
}
