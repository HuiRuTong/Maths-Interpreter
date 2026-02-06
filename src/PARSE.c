#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TOKENS.h"
#include "TOKENISE.h"
#include "PARSE.h"
#include "ERR.h"
#include <stdio.h>

static void advanceToken(Interpreter *interpreter) {
    if ((interpreter->current).type != END) {
        (interpreter->pos)++;
        (interpreter->current) = tokenise(interpreter);
    }
}

static int isValid(FuncType func_type, double arg) {
    switch (func_type) {
    case SQRT:
        if (arg >= 0) {return 1;}
        break;
    case LOG:
        if (arg > 0) {return 1;}
        break;
    case ATRIG:
        if (abs(arg) <= 1) {return 1;}
        break;
    case ACOSH:
        if (arg >= 1) {return 1;}
        break;
    case ATANH:
        if (abs(arg) < 1) {return 1;}
        break;
    default:
        errFunc(-1, NULL);
        break;
    }
    errFunc(INVALID_ARGUMENT, NULL);
}

double term(Interpreter *interpreter) {
    double result;

    if ((interpreter->current).type == NUMERICAL) {
        result = atof((interpreter->current).value);
        advanceToken(interpreter);
    // This if should only execute if there's nothing at the start of an equation
    } else if ((interpreter->current).type == OPERATION) {
        if (!strcmp((interpreter->current).value, "+") || !strcmp((interpreter->current).value, "-")) {
            result = 0; // To handle cases like +5-8 and -5*4
        } else {
            errFunc(INVALID_OPERATOR_COMBINATION, NULL);
        }
    // Functions are treated as terms since they only act on one variable
    } else if ((interpreter->current).type == FUNCTION) {
        if (!strcmp((interpreter->current).value, "sqrt")) {
            advanceToken(interpreter);
            double arg = addSub(interpreter);

            if (isValid(SQRT, arg)) {result = sqrt(arg);};
        } else if (!strcmp((interpreter->current).value, "cbrt")) {
            advanceToken(interpreter);
            result = cbrt(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "ln")) {
            advanceToken(interpreter);

            double arg = addSub(interpreter);
            if (isValid(LOG, arg)) {result = log(arg);}
        } else if (!strcmp((interpreter->current).value, "log")) {
            advanceToken(interpreter);

            double arg = addSub(interpreter);
            if (isValid(LOG, arg)) {result = log10(arg);}
        } else if (!strcmp((interpreter->current).value, "ceil")) {
            advanceToken(interpreter);
            result = ceil(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "floor")) {
            advanceToken(interpreter);
            result = floor(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "sin")) {
            advanceToken(interpreter);
            result = sin(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "cos")) {
            advanceToken(interpreter);
            result = cos(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "tan")) {
            advanceToken(interpreter);
            result = tan(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "asin")) {
            advanceToken(interpreter);

            double arg = addSub(interpreter);
            if (isValid(ATRIG, arg)) {result = asin(arg);}
        } else if (!strcmp((interpreter->current).value, "acos")) {
            advanceToken(interpreter);

            double arg = addSub(interpreter);
            if (isValid(ATRIG, arg)) {result = acos(arg);}
        } else if (!strcmp((interpreter->current).value, "atan")) {
            advanceToken(interpreter);
            result = atan(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "sinh")) {
            advanceToken(interpreter);
            result = sinh(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "cosh")) {
            advanceToken(interpreter);
            result = cosh(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "tanh")) {
            advanceToken(interpreter);
            result = tanh(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "asinh")) {
            advanceToken(interpreter);
            result = asinh(addSub(interpreter));
        } else if (!strcmp((interpreter->current).value, "acosh")) {
            advanceToken(interpreter);

            double arg = addSub(interpreter);
            if (isValid(ACOSH, arg)) {result = acosh(arg);}
        } else {
            advanceToken(interpreter);

            double arg = addSub(interpreter);
            if (isValid(ATANH, arg)) {result = atanh(arg);}
        }
    } else if ((interpreter->current).type == PARENTHESES) {
        // This if only executes if ) is present before (
        if (strcmp((interpreter->current).value, "(")) {errFunc(MISSING_PARENTHESES, NULL);}

            advanceToken(interpreter);
            result = addSub(interpreter);
            if (!strcmp((interpreter->current).value, ")")) {
                advanceToken(interpreter);
                return result;
            } else {
                // This checks if there's a ) present after a (
                errFunc(MISSING_PARENTHESES, NULL);
            }
    } else {
        errFunc(-1, NULL);
    }
    
    return result;
}

double exponent(Interpreter *interpreter) {
    double result = term(interpreter);
    
    while (!strcmp((interpreter->current).value, "^")) {
        advanceToken(interpreter);
        double arg = term(interpreter);
        // Checks even roots for potential imaginary numbers
        // I didn't lump this into isValid() since it requires checking 2 arguments
        if ((fmod(1.0 / arg, 2.0) == 0.0) && result < 0) {
            errFunc(INVALID_ARGUMENT, NULL);
        } else {
            result = pow(result, arg);
        }

        if ((interpreter->current).type == END) {
            break;
        }
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