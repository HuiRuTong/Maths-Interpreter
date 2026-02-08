#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TOKENS.h"
#include "TOKENISE.h"
#include "PARSE.h"
#include "ERR.h"

static void advanceToken(Interpreter *interpreter) {
    free((interpreter->current).value);
    (interpreter->pos)++;
    (interpreter->current) = tokenise(interpreter);
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

double term(Interpreter *interpreter, TokenType terminal) {
    /*
        Lumps multiple tokens together to form a "term", which includes:
        - numbers
        - expressions inside of parentheses
        - values returned by functions
    */
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
    // The only thing that differs here is that their parsing stops once it encounters a )
    } else if ((interpreter->current).type == FUNCTION) {
        if (!strcmp((interpreter->current).value, "sqrt")) {
            advanceToken(interpreter);
            double arg = addSub(interpreter, RPAREN);

            if (isValid(SQRT, arg)) {result = sqrt(arg);};
        } else if (!strcmp((interpreter->current).value, "cbrt")) {
            advanceToken(interpreter);
            result = cbrt(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "ln")) {
            advanceToken(interpreter);

            double arg = addSub(interpreter, RPAREN);
            if (isValid(LOG, arg)) {result = log(arg);}
        } else if (!strcmp((interpreter->current).value, "log")) {
            advanceToken(interpreter);

            double arg = addSub(interpreter, RPAREN);
            if (isValid(LOG, arg)) {result = log10(arg);}
        } else if (!strcmp((interpreter->current).value, "ceil")) {
            advanceToken(interpreter);
            result = ceil(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "floor")) {
            advanceToken(interpreter);
            result = floor(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "sin")) {
            advanceToken(interpreter);
            result = sin(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "cos")) {
            advanceToken(interpreter);
            result = cos(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "tan")) {
            advanceToken(interpreter);
            result = tan(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "asin")) {
            advanceToken(interpreter);

            double arg = addSub(interpreter, RPAREN);
            if (isValid(ATRIG, arg)) {result = asin(arg);}
        } else if (!strcmp((interpreter->current).value, "acos")) {
            advanceToken(interpreter);

            double arg = addSub(interpreter, RPAREN);
            if (isValid(ATRIG, arg)) {result = acos(arg);}
        } else if (!strcmp((interpreter->current).value, "atan")) {
            advanceToken(interpreter);
            result = atan(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "sinh")) {
            advanceToken(interpreter);
            result = sinh(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "cosh")) {
            advanceToken(interpreter);
            result = cosh(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "tanh")) {
            advanceToken(interpreter);
            result = tanh(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "asinh")) {
            advanceToken(interpreter);
            result = asinh(addSub(interpreter, RPAREN));
        } else if (!strcmp((interpreter->current).value, "acosh")) {
            advanceToken(interpreter);

            double arg = addSub(interpreter, RPAREN);
            if (isValid(ACOSH, arg)) {result = acosh(arg);}
        } else {
            advanceToken(interpreter);

            double arg = addSub(interpreter, RPAREN);
            if (isValid(ATANH, arg)) {result = atanh(arg);}
        }
        advanceToken(interpreter);
    } else if ((interpreter->current).type == LPAREN) {
            advanceToken(interpreter);
            result = addSub(interpreter, END);
            // This checks if there's a ) present after a (
            if ((interpreter->current).type != RPAREN) {errFunc(MISSING_PARENTHESES, NULL);}
            
            // The token is left at the terminal so no further operations are carried out
            if ((interpreter->current).type != terminal) {advanceToken(interpreter);}
    } else {
        // This is SUPPOSED to only execute if there's a function / operator receives insufficient arguments
        errFunc(MISSING_OPERAND, NULL);
    }
    
    return result;
}

double exponent(Interpreter *interpreter, TokenType terminal) {
    double result = term(interpreter, terminal);
    
    while ((interpreter->current).value[0] == '^'
           && (interpreter->current).type != terminal) {
        advanceToken(interpreter);
        // If an expression ends before the second operand
        if ((interpreter->current).type == terminal) {errFunc(MISSING_OPERAND, NULL);}
        double arg = term(interpreter, terminal);
        // Checks even roots for potential imaginary numbers
        // I didn't lump this into isValid() since it requires checking 2 arguments
        if ((fmod(1.0 / arg, 2.0) == 0.0) && result < 0) {
            errFunc(INVALID_ARGUMENT, NULL);
        } else {
            result = pow(result, arg);
        }
    }
    
    return result;
}

double mulDiv(Interpreter *interpreter, TokenType terminal) {
    double result = exponent(interpreter, terminal);

    // Evaluate strings of * and / e.g. 8 * 2 / 4 * 5
    while (((interpreter->current).value[0] == '*' || (interpreter->current).value[0] == '/')
           && (interpreter->current).type != terminal) {
        if ((interpreter->current).value[0] == '*') {
            advanceToken(interpreter);
            if ((interpreter->current).type == terminal) {errFunc(MISSING_OPERAND, NULL);}
            result *= exponent(interpreter, terminal);
        } else {
            advanceToken(interpreter);
            if ((interpreter->current).type == terminal) {errFunc(MISSING_OPERAND, NULL);}
            double divisor = exponent(interpreter, terminal);

            // Yea it's finally fuckin' here; not that anyone is stupid enough to make this error, anyway
            if (divisor) {
                result /= divisor;
            } else {
                errFunc(DIV_BY_ZERO, NULL);
            }
        }
    }

    return result;
}

double addSub(Interpreter *interpreter, TokenType terminal) {
    double result = mulDiv(interpreter, terminal);

    while (((interpreter->current).value[0] == '+' || (interpreter->current).value[0] == '-')
           && (interpreter->current).type != terminal) {
        if ((interpreter->current).value[0] == '+') {
            advanceToken(interpreter);
            if ((interpreter->current).type == terminal) {errFunc(MISSING_OPERAND, NULL);}
            result += mulDiv(interpreter, terminal);
        } else {
            advanceToken(interpreter);
            if ((interpreter->current).type == terminal) {errFunc(MISSING_OPERAND, NULL);}
            result -= mulDiv(interpreter, terminal);
        }
    }

    return result;
}

double parse(Interpreter *interpreter) {
    interpreter->pos = 0;
    interpreter->current = tokenise(interpreter);
    return addSub(interpreter, END);
}