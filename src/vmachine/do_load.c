
#include "vmachine.h"

extern Value registers[16];

// load a variable
bool doLOAD()
{
    uint8_t reg;
    uint32_t idx;
    readInstObj(&reg, sizeof(uint8_t));
    readInstObj(&idx, sizeof(uint32_t));
    TRACE("%s,%d", regToStr(reg), idx);

    Value* val = getValBuf(idx);
    memcpy(&registers[reg & 0x0F], val, sizeof(Value));

    return false;
}

// copy a register to another
bool doLOADR()
{
    uint8_t regs;
    readInstObj(&regs, sizeof(uint8_t));

    uint8_t dest = (regs & 0xF0) >> 4;
    uint8_t src = regs & 0x0F;
    TRACE("%s,%s", regToStr(dest), regToStr(src));

    memcpy(&registers[dest], &registers[src], sizeof(Value));

    return false;
}

// load an immediate value into a register
bool doLOADI()
{
    uint8_t reg;
    Value val;
    readInstObj(&reg, sizeof(uint8_t));
    readInstObj(&val, sizeof(Value));
    TRACE("%s,%s", regToStr(reg), valToStr(&val));

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
    TRACE("%d,%s", idx, regToStr(reg));

    assignVal(idx, getValBuf(idx));

    return false;
}

