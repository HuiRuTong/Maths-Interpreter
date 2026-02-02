#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define EQLEN 100
#define NUMLEN 98
const char *(FUNCLIST[]) = {"sqrt", "cbrt", "ln", "log10", "ceil", "floor",
                            "sin", "cos", "tan", "asin", "acos", "atan"
                            "sinh", "cosh", "tanh", "asinh", "acosh", "atanh"};

typedef enum {
    NUMERICAL,
    OPERATION,
    FUNCTION,
    BRACKETS,
    SPACE,
    END
} TokenTypes;

typedef struct {
    TokenTypes type;
    char *value;
} Token;

typedef struct {
    char *equation;
    size_t pos;
    Token current;
} Interpreter;

/*
    Make an actual error handing function to pass these into; this is rlly ugly
*/
// Error types:
void errMultipleDecimals() {
    printf("You've put too many decimal points in a number.\n");
    exit(1);
}
void errMissingOperand() {
    printf("Seems you've forgotten something at the end.\n");
    exit(1);
}
void errInvalidOperatorCombination() {
    printf("No clue what you mean, bud.\n");
    exit(1);
}
void errUnknownSymbol() {
    printf("You've entered a symbol I don't recognise.\n");
    exit(1);
}
void errUnknownFunction () {
    printf("You've entered a function I don't recognise.\n");
    exit(1);
}
void errDivByZero() {
    printf("Absolutely not!");
    exit(1);
}

char advanceChar(Interpreter *);
void advanceToken(Interpreter *);

Token tokenise(Interpreter *interpreter) {
    /* 
        Reads and tokenises character at pos
    */
    char *current_char = malloc(NUMLEN * sizeof(char));
    Token return_token = {.value = malloc(NUMLEN * sizeof(char))};\

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
        return_token.type = NUMERICAL;\
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

            if (current_char[0] == '.' && ++decimals > 1) {
                free(current_char);
                free(return_token.value);
                errMultipleDecimals();
            }
            strcat(return_token.value, current_char);
        }
        free(current_char);
        return return_token;
    }

    if (ispunct(current_char[0])) {
        return_token.type = OPERATION;
        int sign = 0;
        int muldiv = 0;

        do {
            if (current_char[0] != ' ') {
                // If there's nothing after an operator
                if (current_char[0] == '\n') {
                    free(current_char);
                    free(return_token.value);
                    errMissingOperand();
                }

                // To handle strings of + and -
                if (current_char[0] == '+' || current_char[0] == '-') {
                    if (muldiv) {   // If there was a * or / beforehand, reject the equation
                        free(current_char);
                        free(return_token.value);
                        errInvalidOperatorCombination();
                    }

                    sign = 1;
                    muldiv = 0;
                    if (current_char[0] == '-') {
                        sign *= -1;
                    }
                } else {
                    if (sign) {    // If there was a + or - beforehand, reject the equation
                        free(current_char);
                        free(return_token.value);
                        errInvalidOperatorCombination();
                    }
                    sign = 0;
                    if (++muldiv > 1) {
                        errInvalidOperatorCombination();
                    }
                    /*  I'm thinking of just axxing this part
                        or at least until i get around to
                        implementing PEMDAS
                    */
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
                errUnknownFunction();
            }

            fx[i] = current_char[0];
            fx[i+1] = '\0';

            current_char[0] = advanceChar(interpreter);
        } while (isalpha(current_char[0]));
        (interpreter->pos)--; // Set the position back to the last character of the funciton

        if (strlen(fx) < 3) {
            errUnknownFunction();
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

// DEAL WITH LEAKED MEM!!! //

double parse(Interpreter *interpreter) {
    double result = 0;
    interpreter->current = tokenise(interpreter);

    while ((interpreter->current).type != END) {
        int curr_type = (interpreter->current).type;
        char *curr_value = (interpreter->current).value;

        if ((interpreter->current).type == NUMERICAL) {
            result += atof(curr_value);
        } else if (curr_type == OPERATION) {
            if (!strcmp(curr_value, "+")) {
                advanceToken(interpreter);  // Move to a number
                result += atof((interpreter->current).value);   // Then perform the operation
            } else if (!strcmp((interpreter->current).value, "-")) {
                advanceToken(interpreter);
                result -= atof((interpreter->current).value);
            } else if (!strcmp((interpreter->current).value, "*")) {
                advanceToken(interpreter);
                result *= atof((interpreter->current).value);
            } else {
                advanceToken(interpreter);
                result /= atof((interpreter->current).value);
            }
        } else if ((interpreter->current).type == FUNCTION) {
            // I introduce to you, the If Glorb
            if (0) {

            }
        }

        (interpreter->pos)++;
        interpreter->current = tokenise(interpreter);

    }
    return result;
}

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
