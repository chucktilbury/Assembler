
#include "vmachine.h"
#include "runtime.h"

extern Value registers[16];

bool doTRAP()
{
    uint16_t tno;
    readInstObj(&tno, sizeof(uint16_t));
    TRACE("0x%04X", tno);
    return handleTrap(tno);
}

