/*
 *  This test verifies that all of the arithmetic instructions function.
 */
#include "system.inc"
#include "startup.inc"

_start:

    load    r12,123
    load    r13,2
    mul     r10,r12,r13
    push    r10
    trap    DBG_PRINT

    add     r10,r12,r13
    push    r10
    trap    DBG_PRINT

    sub     r10,r12,r13
    push    r10
    trap    DBG_PRINT

    sub     r10,r13,r12
    push    r10
    trap    DBG_PRINT

    div     r10,r12,r13
    push    r10
    trap    DBG_PRINT

    mod     r10,r12,r13
    push    r10
    trap    DBG_PRINT

    load    r12,(num)123
    load    r13,(number)2
    mul     r10,r12,r13
    push    r10
    trap    DBG_PRINT

    add     r10,r12,r13
    push    r10
    trap    DBG_PRINT

    sub     r10,r12,r13
    push    r10
    trap    DBG_PRINT

    sub     r10,r13,r12
    push    r10
    trap    DBG_PRINT

    div     r10,r12,r13
    push    r10
    trap    DBG_PRINT

    mod     r10,r12,r13
    push    r10
    trap    DBG_PRINT

    return
