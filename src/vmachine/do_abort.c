
#include "vmachine.h"

extern Value registers[16];

// end the vm and print a value. probably a string.
bool doABORT()
{
    uint32_t idx;
    readInstObj(&idx, sizeof(uint32_t));
    printVal(getValBuf(idx));
    printf("\n\n");
    return true;
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
