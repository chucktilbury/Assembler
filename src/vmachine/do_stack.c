
#include "vmachine.h"

extern Value registers[16];

bool doPOP()
{
    uint8_t reg;
    readInstObj(&reg, sizeof(uint8_t));
    TRACE("%s", regToStr(reg));

    Value* val = popVal();
    memcpy(&registers[reg], val, sizeof(Value));

    return false;
}

bool doPUSH()
{
    uint8_t reg;
    readInstObj(&reg, sizeof(uint8_t));
    TRACE("%s", regToStr(reg));

    pushVal(&registers[reg]);

    return false;
}

// TODO: PEEK instruction
