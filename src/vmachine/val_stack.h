#ifndef VAL_STACK_H
#define VAL_STACK_H

void initValStack();
void pushVal(Value val);
Value popVal();
Value peekVal(ValIdx idx);
ValIdx getStackIdx();
void validateStackIdx(ValIdx idx);
void dumpStack();

#endif
