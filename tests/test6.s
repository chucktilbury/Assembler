/*
 * Test the
 * - print and format traps
 * - time traps
 * - load and store
 */
#include "traps.inc"

uint time = 0
string time_str = ""
string str = "the time is {time_str}"


_start:
    trap GET_TIME
    store time, R0
    trap STR_TIME
    trap PRINT
    store time_str, R0
    load r0, str
    trap FMT_STR
    trap PRINT      // print the time in a formatted string
    exit
