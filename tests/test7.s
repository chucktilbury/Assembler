/*
 * Test the JMP instruction.
 */
#include "traps.inc"

string passed = "test passed"
string failed = "test failed"
uint test_addr = 59

// this line is passed througfh the preprocessor
//#line 25 "/home/chuck/Src/goldfish/testing/asm/blart.plart"

_start:
    NOP
    //load r0, destination3
    //trap DBG_PRINT
    //store test_addr, r0
    load R0, passed
    push r0
    jmp destination1
    load r0, failed
    trap PRINT

destination1:
    pop r0
    trap PRINT
    //load r2, destination2
    jmp destination2
    load r0, failed
    trap PRINT

destination2:
    // r0 should still be "passed"
    trap PRINT
    //jmp test_addr
    //load r12, test_addr
    jmp destination3
    nop nop
    load r0, failed
    trap PRINT
nop nop
destination3:
nop nop

    trap PRINT

_end:
    exit
