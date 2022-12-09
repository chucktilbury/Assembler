#ifndef _ASSEMBLER_ARITH_H
#define _ASSEMBLER_ARITH_H

void addNum(Value* dest, Value* left, Value* right);
void subNum(Value* dest, Value* left, Value* right);
void mulNum(Value* dest, Value* left, Value* right);
void divNum(Value* dest, Value* left, Value* right);
void negNum(Value* dest, Value* val);

#endif /* _ASSEMBLER_ARITH_H */
