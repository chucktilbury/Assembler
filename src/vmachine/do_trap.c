
#include "vmachine.h"

extern Value registers[16];

bool doTRAP()
{
    uint16_t tno;
    readInstObj(&tno, sizeof(uint16_t));

    return false;
}

