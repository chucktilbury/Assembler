
#include "vmachine.h"

extern Value registers[16];

static inline bool doPOP()
{
    uint8_t reg;
    READ_OBJ(reg, uint8_t);
    TRACE("%s", regToStr(reg));

    Value val = popVal();
    memcpy(&registers[reg], &val, sizeof(Value));

    return false;
}

static inline bool doPUSH()
{
    uint8_t reg;
    READ_OBJ(reg, uint8_t);
    TRACE("%s", regToStr(reg));

    pushVal(registers[reg]);

    return false;
}

static inline bool doPUSHI()
{
    Value val;
    READ_OBJ(val, Value);
    TRACE("%s", valToStr(&val));

    pushVal(val);

    return false;
}

static inline bool doPUSHV()
{
    ValIdx idx;
    READ_OBJ(idx, ValIdx);
    TRACE("value: %d", idx);

    Value* val = getValTab(idx);
    if(val == NULL)
        fatalError("value index %d yields no value", idx);

    pushVal(*val);

    return false;
}

// The dest register is where the result is placed. The base is the saved
// location in the stack where the function stack starts. The index is the
// offset from the base pointer. The index is subtracted from from the base
// and provides an intuitive way to access stack variables.
static inline bool doPEEK()
{
    uint8_t regs;
    uint16_t idx;
    READ_OBJ(regs, uint8_t);
    READ_OBJ(idx, uint16_t);

    uint8_t dest = LEFT_REG(regs);
    uint8_t base = RIGHT_REG(regs);
    TRACE("%s,%s,%d", regToStr(dest), regToStr(base), idx);

    int index = (registers[base].data.unum-idx)+1;
    Value val = peekVal(index);

    memcpy(&registers[dest], &val, sizeof(Value));

    return false;
}

// Store the current stack index into the specified register.
static inline bool doSIDX()
{
    uint8_t reg;
    READ_OBJ(reg, uint8_t);
    TRACE("%s", regToStr(reg));

    uint8_t idx = reg & 0x0F;
    registers[idx].type = UINT;
    registers[idx].data.unum = getStackIdx()-1;

    return false;
}

