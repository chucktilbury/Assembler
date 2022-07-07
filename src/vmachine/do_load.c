
#include "vmachine.h"

extern Value registers[16];

// load a variable
bool doLOAD()
{
    uint8_t reg;
    uint32_t idx;
    readInstObj(&reg, sizeof(uint8_t));
    readInstObj(&idx, sizeof(uint32_t));

    Value* val = getValBuf(idx);
    memcpy(&registers[reg & 0x0F], val, sizeof(Value));

    return false;
}

// copy a register to another
bool doLOADR()
{
    uint8_t regs;
    readInstObj(&regs, sizeof(uint8_t));

    memcpy(&registers[(regs & 0xF0) >> 4], &registers[regs & 0x0F], sizeof(Value));

    return false;
}

// load an immediate value into a register
bool doLOADI()
{
    uint8_t reg;
    Value val;
    readInstObj(&reg, sizeof(uint8_t));
    readInstObj(&val, sizeof(Value));

    memcpy(&registers[reg & 0x0F], &val, sizeof(Value));

    return false;
}

// store a register into a variable
bool doSTORE()
{
    uint32_t idx;
    uint8_t reg;
    readInstObj(&idx, sizeof(uint32_t));
    readInstObj(&reg, sizeof(uint8_t));

    return false;
}

