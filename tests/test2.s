
/*
 * Test the preprocessor and the ASM shell program.
 */
#include "traps.inc"

string thevar = "some string"

load r0, 0xdeadbeef
push r0
load r0, thevar
trap PRINT
pop r0
trap PRINT
addr_of_the_exit_instr:
exit
