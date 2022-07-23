
#include "vmachine.h"

extern Value registers[16];

// load a variable
static inline bool doLOAD()
{
    uint8_t reg;
    uint32_t idx;
    //readInstObj(&reg, sizeof(uint8_t));
    READ_OBJ(reg, uint8_t);
    //readInstObj(&idx, sizeof(uint32_t));
    READ_OBJ(idx, uint32_t);

    //LOG(6, "load index: %d", idx);
    TRACE("%s,value: %d", regToStr(reg), idx);

    Value* val = getValTab(idx);
    if(val == NULL)
        fatalError("value index %d yields no value", idx);

    memcpy(&registers[reg & 0x0F], val, sizeof(Value));
    registers[reg & 0x0F].isAssigned = true;

    return false;
}

// copy a register to another
static inline bool doLOADR()
{
    uint8_t regs;
    //readInstObj(&reg, sizeof(uint8_t));
    READ_OBJ(regs, uint8_t);

    uint8_t dest = (regs & 0xF0) >> 4;
    uint8_t src = regs & 0x0F;
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
    //readInstObj(&reg, sizeof(uint8_t));
    READ_OBJ(reg, uint8_t);
    //readInstObj(&val, sizeof(Value));
    READ_OBJ(val, Value);
    TRACE("%s,%s", regToStr(reg), valToStr(&val));

    memcpy(&registers[reg & 0x0F], &val, sizeof(Value));
    registers[reg & 0x0F].isAssigned = true;

    return false;
}

// store a register into a variable
static inline bool doSTORE()
{
    uint32_t idx;
    uint8_t reg;
    //readInstObj(&idx, sizeof(uint32_t));
    READ_OBJ(idx, uint32_t);
    //readInstObj(&reg, sizeof(uint8_t));
    READ_OBJ(reg, uint8_t);

    //LOG(6, "store index: %d", idx);
    TRACE("%d,%s", idx, regToStr(reg));

    Value* val = getValTab(idx);
    registers[reg & 0x0F].isAssigned = true;
    memcpy(val, &registers[reg & 0x0F], sizeof(Value));

    return false;
}

