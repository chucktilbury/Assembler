
#include "vmachine.h"

extern Value registers[16];

// load a variable
static inline bool doLOAD()
{
    uint8_t reg;
    ValIdx idx;
    READ_OBJ(reg, uint8_t);
    READ_OBJ(idx, ValIdx);

    TRACE("%s,value: %d", regToStr(reg), idx);

    Value* val = getValTab(idx);
    if(val == NULL)
        fatalError("value index %d yields no value", idx);

    reg = reg & 0x0F;
    memcpy(&registers[reg], val, sizeof(Value));
    registers[reg].isAssigned = true;

    return false;
}

// copy a register to another
static inline bool doLOADR()
{
    uint8_t regs;
    READ_OBJ(regs, uint8_t);

    uint8_t dest = LEFT_REG(regs);
    uint8_t src = RIGHT_REG(regs);
    TRACE("%s,%s", regToStr(dest), regToStr(src));

    memcpy(&registers[dest], &registers[src], sizeof(Value));
    registers[dest].isAssigned = true;

    return false;
}

// load an immediate value into a register
static inline bool doLOADI()
{
    uint8_t reg;
    Value val;
    READ_OBJ(reg, uint8_t);
    READ_OBJ(val, Value);
    TRACE("%s,%s", regToStr(reg), valToStr(&val));

    reg = reg & 0x0F;
    memcpy(&registers[reg], &val, sizeof(Value));
    registers[reg].isAssigned = true;

    return false;
}

// store a register into a variable
static inline bool doSTORE()
{
    uint32_t idx;
    uint8_t reg;
    READ_OBJ(idx, uint32_t);
    READ_OBJ(reg, uint8_t);

    TRACE("%d,%s", idx, regToStr(reg));

    Value* val = getValTab(idx);
    reg = reg & 0x0F;
    registers[reg].isAssigned = true;
    memcpy(val, &registers[reg], sizeof(Value));

    return false;
}

static inline bool doSTOREI()
{
    ValIdx idx;
    Value val;
    READ_OBJ(val, Value);
    READ_OBJ(idx, ValIdx);

    TRACE("value: %d,%s", idx, valToStr(&val));

    // printf("val.type = %s\n", valTypeToStr(val.type));
    // printf("val = %ld\n", val.data.num);

    Value* value = getValTab(idx);
    if(value == NULL)
        fatalError("value index %d yields no value", idx);

    val.isAssigned = true;
    memcpy(value, &val, sizeof(Value));

    return false;
}

