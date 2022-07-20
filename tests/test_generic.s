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
    // store the current clock count on the stack
    trap GET_CLOCK
    pop r8  // this is the start time

    load R0, passed
    push r0
    jmp destination1

    // this is never reached
    load r2, failed
    push r2
    trap PRINT

destination1:
    // passed message
    trap PRINT
    load R0, passed
    push r0
    jmp destination2

    // never reached
    load r0, failed
    push r0
    trap PRINT

destination2:
    // r0 should still be "passed"
    trap PRINT
    load R0, passed
    push r0
    jmp destination3

    // never reached
    nop nop
    load r0, failed
    push r0
    trap PRINT

destination3:
    // should print "test passed"
    trap PRINT

    // store the current ticker on the stack
    push R8         // start ticker is now TOS
    trap GET_CLOCK  // end ticker is on TOS
    trap DIF_CLOCK  // TOS is the diffed timer as a float
    pop  R7
    store timer, r7 // store the diffed timer to use with s formatted string

    load r1,print_timer
    push r1
    trap FMT_STR    // formatted string is on TOS
    trap PRINT      // print it

_end:
    exit
