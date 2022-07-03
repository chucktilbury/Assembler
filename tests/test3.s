
/*
 *  Test simple arithmatic.
 */
#include "traps.inc"

test3:
    load r5, 123
    load r6, 2
    mul  r0,r5,r6
    trap PRINT
    exit
