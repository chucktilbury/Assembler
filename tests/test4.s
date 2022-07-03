
/*
 * test the CALL instruction
 */
#include "traps.inc"

string the_value = "this is a string"
float test_value = 143.3487

_start:
    nop
    load r0,test_value
    load r1,the_value
    call the_func
    trap PRINT  // R0 should have test_value in it
    exit

// this is the calling destination
// int parameter in R1
the_func:
    push r0     // save the trap register
    load r0,r1  // R0 is the parameter for the trap
    trap PRINT
    pop r0      // restore the trap reg
    return
