/*
 * Startup code for a generic application.
 */
#ifndef STARTUP_INC
#define STARTUP_INC

// TODO: add more startup things like argv
_root_entry:
    call    _start  // allows the start function to be a normal function.
    nop
    exit

#endif

