#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TOKENS.h"
#include "ERR.h"

const char *(FUNCLIST[]) = {"sqrt", "cbrt", "ln", "log", "ceil", "floor",
                            "sin", "cos", "tan", "asin", "acos", "atan"
                            "sinh", "cosh", "tanh", "asinh", "acosh", "atanh"};

static void advanceChar(Interpreter *interpreter, char *c) {
    (interpreter->pos)++;
    c[0] = (interpreter->equation)[interpreter->pos];
}

Token tokenise(Interpreter *interpreter) {
    /* 
        Reads and tokenises character at pos
    */
    Token return_token = {.value = malloc(NUMLEN * sizeof(char))};
    return_token.value[0] = (interpreter->equation)[interpreter->pos];
    return_token.value[1] = '\0';
    
    while (return_token.value[0] == ' ') {
        advanceChar(interpreter, return_token.value);
    }

    if (interpreter->pos == strlen(interpreter->equation) - 1) {        
        return_token.type = END;
        return return_token;
    }

    // Special cases where numbers are represented by non digits
    if (return_token.value[0] == 'e') {
        return_token.type = NUMERICAL;
        // Here's a lesson for future me:
        // DO NOT FUCKING ASSIGN A MALLOCED STRING WITH =
        // ONLY ASSIGN EACH INDEX MANUALLY OR USE STRCPY
        // YOU KNOW HOW LONG I SPENT TRYING TO FIX THE LEAKED MEMORY????
        strcpy(return_token.value, "2.71828183");
        return return_token;
    }
    
    if (return_token.value[0] == 'p') {
        advanceChar(interpreter, return_token.value);
        if (return_token.value[0] == 'i') {
            return_token.type = NUMERICAL;
            strcpy(return_token.value, "3.14159265");
            return return_token;
        }
        errFunc(UNKNOWN_SYMBOL, &return_token);
    }

    if (isdigit(return_token.value[0]) || return_token.value[0] == '.') {
        return_token.type = NUMERICAL;
        int decimals = 0;
        int i = 0; // index for return_token.value

        if (return_token.value[0] == '.') {
            decimals += 1;
        }

        char curr_char = (interpreter->equation)[interpreter->pos];
        // Appends other digits onto the first if there's > 1 digit
        while (isdigit((interpreter->equation)[(interpreter->pos) + 1]) ||
              (interpreter->equation)[(interpreter->pos) + 1] == '.' ||
              (interpreter->equation)[(interpreter->pos) + 1] == ' ') {
            
            i++;
            advanceChar(interpreter, &curr_char);

            if (curr_char == ' ') {
                continue;
            } 

            if (curr_char == '.' && ++decimals > 1) {errFunc(MULTIPLE_DECIMALS, &return_token);}

            return_token.value[i] = curr_char;
            return_token.value[i+1] = '\0';
        }
        return return_token;
    }

    // Parentheses are considered punctuation for some reason
    if (return_token.value[0] == '(') {
        return_token.type = LPAREN;
        return return_token;
    }
    if (return_token.value[0] == ')') {
        return_token.type = RPAREN;
        return return_token;
    }

    if (ispunct(return_token.value[0])) {
        return_token.type = OPERATION;
        int sign = 0;
        int muldiv = 0;
        int exp = 0;
    
        char curr_char = (interpreter->equation)[interpreter->pos];
        do {
            if (curr_char != ' ') {
                // If there's nothing after an operator
                if (curr_char == '\n') {errFunc(MISSING_OPERAND, &return_token);}
                // I need to explicitly separate these from operators
                if (curr_char == '(' || curr_char == ')' || curr_char == '.') {break;}

                if (curr_char == '+' || curr_char == '-') { // To handle strings of + and -
                    // If there was a * / or ^ beforehand, reject the equation
                    if (muldiv || exp) {errFunc(INVALID_OPERATOR_COMBINATION, &return_token);}
                    sign = 1;
                    muldiv = 0;
                    exp = 0;

                    if (curr_char == '-') {sign *= -1;}
                } else if (curr_char == '*' || curr_char == '/') {
                    // If there was a + -  ^ beforehand, reject the equation
                    if (sign || exp) {errFunc(INVALID_OPERATOR_COMBINATION, &return_token);}

                    sign = 0;
                    exp = 0;

                    if (++muldiv > 1) {errFunc(INVALID_OPERATOR_COMBINATION, &return_token);}
                } else if (return_token.value[0] == '^') {
                    // Take a wild guess as to why this is here
                    if (sign || muldiv) {errFunc(INVALID_OPERATOR_COMBINATION, &return_token);}
                    
                    sign = 0;
                    muldiv = 0;

                    if (++exp > 1) {errFunc(INVALID_OPERATOR_COMBINATION, &return_token);}
                } else {errFunc(UNKNOWN_SYMBOL, &return_token);}
            }

            advanceChar(interpreter, &curr_char);
        } while (ispunct(curr_char) || isspace(curr_char));
        (interpreter->pos)--;
        
        if (sign > 0) {
            strcpy(return_token.value, "+");
        } else if (sign < 0) {
            strcpy(return_token.value, "-");
        }

        return return_token;
    }

    if (isalpha(return_token.value[0])) {
        return_token.type = FUNCTION;
        char curr_char = (interpreter->equation)[interpreter->pos];
        int i = 0; // Index for return_token.value

        do {
            if (i > 5) {errFunc(UNKNOWN_SYMBOL, &return_token);} // No function is longer than 5 characters

            return_token.value[i] = curr_char;
            return_token.value[i+1] = '\0';

            advanceChar(interpreter, &curr_char);
            i++;
        } while (isalpha(curr_char));
        if (curr_char != '(') {errFunc(MISSING_PARENTHESES, &return_token);}
        (interpreter->pos)--; // Set the position back to the last character of the funciton

        if (strlen(return_token.value) < 2) {errFunc(UNKNOWN_SYMBOL, &return_token);} // No function is less than 2 characters

        for (int i = 0; i < 18; i++) {
            if (!strcmp(return_token.value, FUNCLIST[i])) {
                return return_token;
            }
        }
        errFunc(UNKNOWN_SYMBOL, &return_token);
    }
}