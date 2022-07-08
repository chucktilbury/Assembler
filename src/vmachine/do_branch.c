
#include "vmachine.h"

extern Value registers[16];

bool doCALL()
{
    uint32_t idx;
    readInstObj(&idx, sizeof(uint32_t));
    TRACE("%04d", idx);

    Value* val = createValue(UINT);
    val->data.unum = getIndex();

    pushVal(val);

    return setIndex(idx);
}

bool doJMP()
{
    uint32_t idx;
    readInstObj(&idx, sizeof(uint32_t));
    TRACE("%04d", idx);

    return setIndex(idx);
}

bool doBR()
{
    uint32_t idx;
    readInstObj(&idx, sizeof(uint32_t));
    TRACE("%04d", idx);

    return false;
}

bool doRETURN()
{
    Value* val = popVal();
    if(val->type != UINT && val->type != INT) {
        runtimeError("invalid type for return instruction: %d", val->type);
        return true;
    }
    else
        return setIndex(val->data.unum);
}

