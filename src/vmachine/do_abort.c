
#include "vmachine.h"

extern Value registers[16];

// abort the vm and print an error
bool doABORT()
{
    uint32_t idx;
    readInstObj(&idx, sizeof(uint32_t));
    printVal(getValBuf(idx));
    return true;
}

// exit the VM normally.
bool doEXIT()
{
    return true;
}

// do nothing at all
bool doNOP()
{
    return false;
}
