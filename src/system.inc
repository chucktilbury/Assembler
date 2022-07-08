/*
 * This is the system include file. It contains numerical definitions for all
 * the valid trap numbers. This much be defines and not an enum because it is
 * included by the assembler source file and enums are not a part of that
 * language, but they are a part of the assembly language.
 */

 #define DBG_PRINT  0
 #define PRINT      1
 #define GET_TIME   2
 #define SHOW_TIME  3
 #define GET_CLOCK  4
 #define DIF_CLOCK  5