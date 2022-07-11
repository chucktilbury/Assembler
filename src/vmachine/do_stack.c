
#include "vmachine.h"

extern Value registers[16];

bool doPOP()
{
    uint8_t reg;
    readInstObj(&reg, sizeof(uint8_t));
    TRACE("%s", regToStr(reg));

    Value val = popVal();
    memcpy(&registers[reg], &val, sizeof(Value));

    return false;
}

bool doPUSH()
{
    uint8_t reg;
    readInstObj(&reg, sizeof(uint8_t));
    TRACE("%s", regToStr(reg));

    pushVal(registers[reg]);

    return false;
}

// The dest register is where the result is placed. The base is the saved
// location in the stack where the function stack starts. The index is the
// offset from the base pointer. The index is subtracted from from the base
// and provides an intuitive way to access stack variables.
bool doPEEK()
{
    uint8_t regs;
    uint16_t idx;
    readInstObj(&regs, sizeof(regs));
    readInstObj(&idx, sizeof(idx));

    uint8_t dest = (regs & 0xF0) >> 4;
    uint8_t base = (regs & 0x0F);
    TRACE("%s,%s,%d", regToStr(dest), regToStr(base), idx);

    int index = (registers[base].data.unum-idx)+1;
    Value val = peekVal(index);

    memcpy(&registers[dest], &val, sizeof(Value));

    return false;
}

// Store the current stack index into the specified register.
bool doSIDX()
{
    uint8_t reg;
    readInstObj(&reg, sizeof(reg));
    TRACE("%s", regToStr(reg));

    uint8_t idx = reg & 0x0F;
    registers[idx].type = UINT;
    registers[idx].data.unum = getStackIdx()-1;

    return false;
}

