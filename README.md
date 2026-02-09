# Maths-Interpreter
An interpreter for the user to enter basic maths expressions.

## Syntax:
1. Operators:
    - Plus (\+) & minus (\-) can be combined indefinitely
    - Times (\*), divide (/), & exponentiation (^) cannot be combined with any other operator. If you wish to enter a negative exponent, use parentheses _e.g. e^(-2)_.
    - If no operand appears before an + or -, it is assumed to be 0.

2. Spaces:
    - Between numbers and operations are OK but all functions and their arguments shouldn't be separated.

3. Decimals:
    - Any missing digits before or after adecimal point (.) is assumed to be 0.

4. Functions:
    - See list below for all defined functions
    - Function arguments must be inside parentheses
    - Trig functions are all in radians. I'm not implementing a way to switch to degrees because radians are far superior.

5. Special symbols:
    - The interpreter recognises "e" and "pi" as 2.71828 and 3.14159 respectively


## List of Defined Functions
| Recognised Syntax | Description                                  |
| :----------------:| :--------------------------------------------|
| sqrt              | Square root.                                 |
| cbrt              | Cube root.                                   |
| ln                | Natural log.                                 |
| log               | Log base 10.                                 |
| ceil              | Rounds up a number to the closest integer.   |
| floor             | Rounds down a number to the closest integer. |
| sin               | Sine.                                        |
| cos               | Cosine.                                      |
| tan               | Tangent.                                     |
| asin              | Inverse sine.                                |
| acos              | Inverse cosine.                              |
| atan              | Inverse tangent.                             |
| sinh              | Hyperbolic sine.                             |
| cosh              | Hyperbolic cosine.                           |
| tanh              | Hyperbolic tangent.                          |
| asinh             | Inverse hyperbolic sine.                     |
| acosh             | Inverse hyperbolic cosine.                   |
| atanh             | Inverse hyperbolic tangent.                  |