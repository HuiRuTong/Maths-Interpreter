#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TOKENS.h"
#include "TOKENISE.h"
#include "PARSE.h"
#include "ERR.h"

static void advanceToken(Interpreter *interpreter) {
    (interpreter->pos)++;
    (interpreter->current) = tokenise(interpreter);
}

double term(Interpreter *interpreter) {
    double result;

    if ((interpreter->current).type == NUMERICAL) {
        result = atof((interpreter->current).value);
        return result;
    }
    
    if (!strcmp((interpreter->current).value, "(")) {
        advanceToken(interpreter);
        result = addSub(interpreter);
        if (!strcmp((interpreter->current).value, ")")) {
            return result;
        } else {
            // If a parenthesis isn't closed
            errFunc(MISSING_PARENTHESES, NULL);
        }
    }
}

double exponent(Interpreter *interpreter) {
    double result;

    // This if should only execute if there's nothing at the start of an equation
    if ((interpreter->current).type == OPERATION) {
        if (!strcmp((interpreter->current).value, "+") || !strcmp((interpreter->current).value, "-")) {
            result = 0; // To handle cases like +5-8 and -5*4
        } else {
            errFunc(INVALID_OPERATOR_COMBINATION, NULL);
        }
    } else {
        result = term(interpreter);
        advanceToken(interpreter);
    }
    
    while (!strcmp((interpreter->current).value, "^")) {
        advanceToken(interpreter);
        result = pow(result, term(interpreter));

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
    while (!strcmp((interpreter->current).value, "*") || !strcmp((interpreter->current).value, "/")) {
        if (!strcmp((interpreter->current).value, "*")) {
            advanceToken(interpreter);
            result *= exponent(interpreter);
        } else {
            advanceToken(interpreter);
            double divisor = exponent(interpreter);

            // Yea it's finally fuckin' here; not that anyone is stupid enough to make this error, anyway
            if (divisor) {
                result /= divisor;
            } else {
                errFunc(DIV_BY_ZERO, NULL);
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

    while (!strcmp((interpreter->current).value, "+") || !strcmp((interpreter->current).value, "-")) {
        if (!strcmp((interpreter->current).value, "+")) {
            advanceToken(interpreter);
            result += mulDiv(interpreter);
        } else {
            advanceToken(interpreter);
            result -= mulDiv(interpreter);
        }

        if ((interpreter->current).type == END) {
            break;
        }
    }

    return result;
}

double parse(Interpreter *interpreter) {
    interpreter->pos = 0;
    interpreter->current = tokenise(interpreter);
    return addSub(interpreter);
}