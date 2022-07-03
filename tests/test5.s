/*
 * Test string formatting
 */
#include "traps.inc"

int value1 = 1234
string value2 = "ploop"
string formatted = "this is a {value1} string with a {value2} and no nl"

_start:
    load r0, formatted
    trap FMT_STR
    trap PRINT
    exit
