
#include "vmachine.h"

extern Value registers[16];

static inline bool doCALL()
{
    uint32_t idx;
    //readInstObj(&idx, sizeof(uint32_t));
    READ_OBJ(idx, uint32_t);
    TRACE("%04d", idx);

    //Value* val = createValue(UINT);
    Value val;
    val.type = UINT;
    //val->data.unum = getIndex();
    val.data.unum = GET_IP();

    pushVal(val);

    SET_IP(idx);
    //return setIndex(idx);
    return false;
}

static inline bool doJMP()
{
    uint32_t idx;
    //readInstObj(&idx, sizeof(uint32_t));
    READ_OBJ(idx, uint32_t);
    TRACE("%04d", idx);

    SET_IP(idx);
    //return setIndex(idx);
    return false;
}

static inline bool doBR()
{
    uint32_t idx;
    //readInstObj(&idx, sizeof(uint32_t));
    READ_OBJ(idx, uint32_t);
    TRACE("%04d", idx);

    if(getZflag()) {
        SET_IP(idx);
        return false;
        //return setIndex(idx);
    }
    else
        return false;
}

static inline bool doRETURN()
{
    Value val = popVal();
    if(val.type != UINT && val.type != INT) {
        runtimeError("invalid type for return instruction: %d", val.type);
        return true;
    }
    else
        SET_IP((uint32_t)val.data.unum);
        //return setIndex(val.data.unum);
    return false;
}

