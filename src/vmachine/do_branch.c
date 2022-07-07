
#include "vmachine.h"

extern Value registers[16];

bool doCALL()
{
    uint32_t idx;
    readInstObj(&idx, sizeof(uint32_t));

    return false;
}

bool doJMP()
{
    uint32_t idx;
    readInstObj(&idx, sizeof(uint32_t));

    return false;
}

bool doBR()
{
    uint32_t idx;
    readInstObj(&idx, sizeof(uint32_t));

    return false;
}

bool doRETURN()
{
    return false;
}

