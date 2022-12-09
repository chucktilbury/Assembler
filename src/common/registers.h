#ifndef _COMMON_REGISTERS_H
#define _COMMON_REGISTERS_H

typedef enum {
    REG_0,
    REG_1,
    REG_2,
    REG_3,
    REG_4,
    REG_5,
    REG_6,
    REG_7,
    REG_8,
    REG_9,
    REG_10,
    REG_11,
    REG_12,
    REG_13,
    REG_14,
    REG_15,
} Register;


typedef uint8_t Reg;

const char* regToStr(uint8_t reg);

#endif /* _COMMON_REGISTERS_H */
