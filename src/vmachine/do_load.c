
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

    reg = reg & 0x0F;
    memcpy(&registers[reg], val, sizeof(Value));
    registers[reg].isAssigned = true;

    return false;
}

// copy a register to another
static inline bool doLOADR()
{
    uint8_t regs;
    //readInstObj(&reg, sizeof(uint8_t));
    READ_OBJ(regs, uint8_t);

    uint8_t dest = LEFT_REG(regs); //(regs & 0xF0) >> 4;
    uint8_t src = RIGHT_REG(regs); //regs & 0x0F;
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
    //readInstObj(&idx, sizeof(uint32_t));
    READ_OBJ(idx, uint32_t);
    //readInstObj(&reg, sizeof(uint8_t));
    READ_OBJ(reg, uint8_t);

    //LOG(6, "store index: %d", idx);
    TRACE("%d,%s", idx, regToStr(reg));

    Value* val = getValTab(idx);
    reg = reg & 0x0F;
    registers[reg].isAssigned = true;
    memcpy(val, &registers[reg], sizeof(Value));

    return false;
}

