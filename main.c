#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define EQLEN 100
#define NUMLEN 98

typedef enum {
    NUMERICAL,
    OPERATION,
    BRACKETS,
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

    if (isdigit(current_char[0])) {
        return_token.type = NUMERICAL;
        int decimals = 0;

        // Appends other digits onto the first
        while (isdigit((interpreter->equation)[(interpreter->pos) + 1]) ||
               (interpreter->equation)[(interpreter->pos) + 1] == '.') {
            (interpreter->pos)++;
            current_char[0] = (interpreter->equation)[interpreter->pos];

            if (!strcmp(current_char, ".") && ++decimals > 1) {
                free(current_char);
                free(return_token.value);
                printf("You typed two decimal points in a number.");
                exit(1);
            }
            strcat(return_token.value, current_char);
        }
        free(current_char);
        return return_token;
    }

    if (ispunct(current_char[0])) {
        return_token.type = OPERATION;
        int sign = 1;

        do {
            if (!(strcmp(current_char, "+"))) {
                sign *= 1;
            } else if (!(strcmp(current_char, "-"))) {
                sign *= -1;
            } else {
                // As long as there's an operator next to a * or /, reject it
                if (ispunct((interpreter->equation)[(interpreter->pos) + 1]) ||
                    ispunct((interpreter->equation)[(interpreter->pos) - 1])) {
                    sign = 0;
                    break;
                }
                // Don't change * and /
                return return_token;
            }
            (interpreter->pos)++;
            current_char[0] = (interpreter->equation)[interpreter->pos];
        } while (ispunct((interpreter->equation)[interpreter->pos]));

        (interpreter->pos)--;
        if (sign > 0) {
            return_token.value = "+";
        } else if (sign < 0) {
            return_token.value = "-";
        } else {
            free(current_char);
            free(return_token.value);
            printf("Don't mix and match + - with * /! Also, no mixing * /!\n");
            exit(1);
        }

        free(current_char);
        return return_token;
    }

    if (!strcmp(current_char, "(") || (!strcmp(current_char, ")"))) {
        return_token.type = BRACKETS;
        free(current_char);
        return return_token;
    }

    printf("I don't recognise that!!\n");
    exit(1);
}

// DEAL WITH LEAKED MEM!!! //

double parse(Interpreter *interpreter) {
    double result = 0;
    interpreter->current = tokenise(interpreter);

    while ((interpreter->current).type != END) {
        if ((interpreter->current).type == NUMERICAL) {
            result += atof((interpreter->current).value);
        } else {
            if (!strcmp((interpreter->current).value, "+")) {
                (interpreter->pos)++;
                interpreter->current = tokenise(interpreter);

                result += atof((interpreter->current).value);
            } else if (!strcmp((interpreter->current).value, "-")) {
                (interpreter->pos)++;
                interpreter->current = tokenise(interpreter);

                result -= atof((interpreter->current).value);
            } else if (!strcmp((interpreter->current).value, "*")) {
                (interpreter->pos)++;
                interpreter->current = tokenise(interpreter);

                result *= atof((interpreter->current).value);
            } else {
                (interpreter->pos)++;
                interpreter->current = tokenise(interpreter);

                result /= atof((interpreter->current).value);
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
    fgets(input, 100, stdin);
    test.equation = removeSpaces(input);
    printf("%lf\n", parse(&test));
}

// Returns a str with its space removed
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
