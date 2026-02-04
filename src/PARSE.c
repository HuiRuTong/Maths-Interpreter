#include <string.h>
#include <math.h>
#include "TOKENS.h"
#include "TOKENISE.h"
#include "ERR.h"

static void advanceToken(Interpreter *interpreter) {
    (interpreter->pos)++;
    (interpreter->current) = tokenise(interpreter);
}

double exponent(Interpreter *interpreter) {
    double result;
    if (interpreter->pos == 0 && (interpreter->current).type == OPERATION) {
        if (!strcmp((interpreter->current).value, "+") || !strcmp((interpreter->current).value, "-")) {
            result = 0; // To handle cases like +5-8 and -5*4
        } else {
            errFunc(INVALID_OPERATOR_COMBINATION, NULL, NULL);
        }
    } else {
        result = atof((interpreter->current).value);
        advanceToken(interpreter);
    }
    
    while (!strcmp((interpreter->current).value, "^")) {
        advanceToken(interpreter);
        result = pow(result, atof((interpreter->current).value));

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
        if (!strcmp((interpreter->current).value, "*")) {
            advanceToken(interpreter);
            result *= exponent(interpreter);
        } else {
            advanceToken(interpreter);
            result /= exponent(interpreter);
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