#ifndef ARITH_H
#define ARITH_H

void doNEG(uint8_t dest, uint8_t reg);
void doADD(uint8_t dest, uint8_t left, uint8_t right);
void doSUB(uint8_t dest, uint8_t left, uint8_t right);
void doMUL(uint8_t dest, uint8_t left, uint8_t right);
void doDIV(uint8_t dest, uint8_t left, uint8_t right);
void doMOD(uint8_t dest, uint8_t left, uint8_t right);

#endif
