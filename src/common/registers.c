
#include "common.h"

const char* regToStr(uint8_t reg)
{
    return (reg == REG_1)? "R1":
            (reg == REG_0)? "R0":
            (reg == REG_2)? "R2":
            (reg == REG_3)? "R3":
            (reg == REG_4)? "R4":
            (reg == REG_5)? "R5":
            (reg == REG_6)? "R6":
            (reg == REG_7)? "R7":
            (reg == REG_8)? "R8":
            (reg == REG_9)? "R9":
            (reg == REG_10)? "R10":
            (reg == REG_11)? "R11":
            (reg == REG_12)? "R12":
            (reg == REG_13)? "R13":
            (reg == REG_14)? "R14":
            (reg == REG_15)? "R15": "UNKNOWN";
}

