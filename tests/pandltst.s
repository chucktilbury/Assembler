/*
 * This is the push and load test. It's created to test the new PUSHI, PUSHV,
 * and STOREI instructions.
 */
#include "system.inc"
#include "startup.inc"

num test

_start:
    store   test,123    // storeI
    push    test        // pushv
    trap    DBG_PRINT   // print (INT)123
    //pop     r15         // balance the stack
    push    234         // pushI
    trap    DBG_PRINT   // print (INT)234
    return

nop
nop
// end of file
