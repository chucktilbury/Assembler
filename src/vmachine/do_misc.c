
#include "vmachine.h"
#include "runtime.h"

extern Value registers[16];

static inline bool doTRAP()
{
    uint16_t tno;
    READ_OBJ(tno, uint16_t);
    TRACE("0x%04X", tno);
    return handleTrap(tno);
}

// exit the VM normally.
static inline bool doEXIT()
{
    // end execution
    return true;
}

// do nothing at all
static inline bool doNOP()
{
    // continue execution
    return false;
}
