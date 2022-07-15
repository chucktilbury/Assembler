/*
 * Test the JMP instruction.
 */
#include "system.inc"

string passed = "test passed"
string failed = "test failed"
float timer = 0.0
string print_timer = "elapsed time is {timer} seconds"
uint test_addr = 59

// this line is passed through the preprocessor
//#line 25 "/home/chuck/Src/goldfish/testing/asm/blart.plart"

_start:
    NOP
    trap GET_CLOCK
    load r8,r0
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
    jmp destination3
    nop nop
    load r0, failed
    trap PRINT

destination3:
    // should print "test passed"
    trap PRINT
    trap GET_CLOCK
    load r2,r0
    load r1,r8
    trap DIF_CLOCK
    store timer, r0
    load r1,print_timer
    trap FMT_STR
    trap PRINT

_end:
    exit
