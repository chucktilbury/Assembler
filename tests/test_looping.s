/*
 * This test covers comparisons and branching.
 *
 *  Calls a function 10 times and prints the counter.
 */
#include "system.inc"

string out_str = "the index is {counter}"
int counter

_start:

    load    r0,10   // the limit
    load    r1,0    // the counter
    load    r2,1    // increment

    jmp     _bottom_of_loop
_top_of_loop:
    push    r1
    sidx    r15
    call    _the_func
    pop     r15     // finished with R15

_bottom_of_loop:
    add     r1,r1,r2
    geq     r0,r1   // if limit >= counter
    br      _top_of_loop    // then branch
    jmp     _finished

_the_func:
    peek    r12,r15,1
    store   counter,r12
    load    r11,out_str
    push    r11
    trap    FMT_STR
    trap    PRINT
    return

_finished:
    exit

