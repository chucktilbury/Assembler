
#include "vmachine.h"

extern Value registers[16];

bool doPOP()
{
    uint8_t reg;
    readInstObj(&reg, sizeof(uint8_t));

    return false;
}

bool doPUSH()
{
    uint32_t idx;
    readInstObj(&idx, sizeof(uint32_t));

    return false;
}

// TODO: PEEK instruction
