/*
 * Test the peek and sidx instructions.
 */
#include "system.inc"

int value1 = 123
int value2

_start:
    load    r12,value1
    load    r11,23
    add     r10,r11,r12     // R10 = 146
    store   value2,r10

    // prepare to enter the function
    push    r10             // first func parameter = 146
    load    r0,value1
    push    r0              // second func parameter = 123
    sidx    R12             // base of stack is in R12
    call    _the_func       // Call the function

    // clean up after the function call
    pop     r0
    pop     r0

    // this is here for after the call
    nop
    nop
    nop
    exit

_the_func:
    peek    r9,r12,1
    peek    r10,r12,2
    add     r8,r9,r10
    push    r8
    trap    DBG_PRINT   // should print 269
    return



