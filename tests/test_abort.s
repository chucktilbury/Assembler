/*
 * Test aborting. Abort accepts a register parameter and prints a value that
 * is given by a register.
 */
#include "system.inc"

//string reason
string abort_msg = "error: '{reason}': cannot continue"
string reason

_start:
    load r12,"test abort"
    store reason,r12

    // format string is in R1 and result placed in R0
    load r0,abort_msg
    push r0

    // format string expects the string to format on the top of the stack
    // and it pushes the result back on the top of stack.
    trap FMT_STR

    // Abort expects a message on the top of stack execution stops with
    // the abort trap.
    trap ABORT


    // unreacable
    trap DBG_PRINT  // should not happen
    exit
