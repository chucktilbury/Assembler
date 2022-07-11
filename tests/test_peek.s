/*
 * Test the peek and sidx instructions.
 */
#include "system.inc"

int value1 = 123
int value2

_start:
    load r0,value1
    store value2,r0
    load r5, 72
    load r6,value2
    add r0,r5,r6
    trap PRINT // print 195
    call _func
    nop
    nop
    nop
    exit

_func:
    load r0,(UINT)8080
    store value2,r0
    push r0  // index 2 = 8080
    trap DBG_PRINT

    load r12, value1
    push r12 // index 1 = 123
    load r0,r12
    trap DBG_PRINT

    sidx r15
    load r0,r15
    trap PRINT

_func_entry:
    peek r0,r15,1
    trap DBG_PRINT

    peek r0,r15,2
    trap DBG_PRINT

    load r0,"string"
    pop r0
    trap DBG_PRINT
    pop r0
    trap DBG_PRINT
    return
