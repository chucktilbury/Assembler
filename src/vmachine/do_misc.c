
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

// exit the VM normally.
bool doEXIT()
{
    // end execution
    return true;
}

// do nothing at all
bool doNOP()
{
    // continue execution
    return false;
}
