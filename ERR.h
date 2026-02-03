#ifndef ERR_H
#define ERR_H

void errFunc(void *, char *, char *);

void errMultipleDecimals();
void errMissingOperand();
void errInvalidOperatorCombination();
void errUnknownSymbol();
void errUnknownFunction ();
void errDivByZero();

#endif