
#include "vmachine.h"
#include <math.h>

extern Value registers[16];
extern int errors;

#define _div_by_zero(r) do { \
        switch((r).type) { \
            case INT: \
                if((r).data.num == 0) { \
                    fatalError("divide by zero"); \
                } \
                break; \
            case UINT: \
                if((r).data.unum == 0) { \
                    fatalError("divide by zero"); \
                } \
                break; \
            case FLOAT: \
                if((r).data.fnum == 0.0) { \
                    fatalError("divide by zero"); \
                } \
                break; \
            case BOOL: \
            case STRING: \
            case ERROR: \
                break; \
            default: \
                fatalError("unknown register type: %d", (r).type); \
        } \
    } while(false)


#define _operation(d, l, r, op) do { \
        switch((l).type) { \
            case INT: \
                switch((r).type) { \
                    case INT: \
                        (d).type = INT; \
                        (d).data.num = (l).data.num op (r).data.num; \
                        break; \
                    case UINT: \
                        (d).type = INT; \
                        (d).data.num = (l).data.num op (r).data.unum; \
                        break; \
                    case FLOAT: \
                        (d).type = FLOAT; \
                        (d).data.fnum = (l).data.num op (r).data.fnum; \
                        break; \
                    case BOOL: \
                    case STRING: \
                    case ERROR: \
                        runtimeError("cannot perform a '%s' on a %s", #op, valTypeToStr((l).type)); \
                        break; \
                    default: \
                        fatalError("unknown register type: %d", (r).type); \
                } \
                break; \
            case UINT: \
                switch((r).type) { \
                    case INT: \
                        (d).type = INT; \
                        (d).data.num = (l).data.unum op (r).data.num; \
                        break; \
                    case UINT: \
                        (d).type = UINT; \
                        (d).data.unum = (l).data.unum op (r).data.unum; \
                        break; \
                    case FLOAT: \
                        (d).type = FLOAT; \
                        (d).data.fnum = (l).data.unum op (r).data.fnum; \
                        break; \
                    case BOOL: \
                    case STRING: \
                    case ERROR: \
                        runtimeError("cannot perform a '%s' on a %s", #op, valTypeToStr((l).type)); \
                        break; \
                    default: \
                        fatalError("unknown register type: %d", (r).type); \
                } \
                break; \
            case FLOAT: \
                switch((r).type) { \
                    case INT: \
                        (d).type = FLOAT; \
                        (d).data.fnum = (l).data.fnum op (r).data.num; \
                        break; \
                    case UINT: \
                        (d).type = FLOAT; \
                        (d).data.fnum = (l).data.fnum op (r).data.unum; \
                        break; \
                    case FLOAT: \
                        (d).type = FLOAT; \
                        (d).data.fnum = (l).data.fnum op (r).data.fnum; \
                        break; \
                    case BOOL: \
                    case STRING: \
                    case ERROR: \
                        runtimeError("cannot perform a '%s' on a %s", #op, valTypeToStr((l).type)); \
                        break; \
                    default: \
                        fatalError("fatal error: unknown register type: %d", (r).type); \
                } \
                break; \
            case BOOL: \
            case STRING: \
            case ERROR: \
                    runtimeError("cannot perform a '%s' on a %s", #op, valTypeToStr((l).type)); \
                errors++; \
                break; \
            default: \
                fatalError("unknown register type: %d", (l).type); \
        } \
    } while(false)

static inline bool doNEG()
{
    uint8_t regs;
    //readInstObj(&regs, sizeof(uint8_t));
    READ_OBJ(regs, uint8_t);

    uint8_t dest = RIGHT_REG(regs); //(regs & 0x00F0) >> 4;
    uint8_t reg = LEFT_REG(regs); //regs & 0x000F;
    TRACE("%s,%s", regToStr(dest), regToStr(reg));

    //Value* res = &registers[dest];
    //Value* val = &registers[reg];

    registers[dest].type = registers[reg].type;
    switch(registers[reg].type) {
        case INT: registers[dest].data.num = -registers[reg].data.num; break;
        case UINT: registers[dest].data.unum = -registers[reg].data.unum; break;
        case FLOAT: registers[dest].data.fnum = -registers[reg].data.fnum; break;
        case BOOL: registers[dest].data.bval = registers[reg].data.bval? false: true; break;
        case STRING:
            registers[dest].type = ERROR;
            runtimeError("cannot negate a string");
            break;
        case ERROR: /* do nothing */ break;
        default:
            fatalError("unknown register type: %d", registers[reg].type);
    }

    return false;
}

static inline bool doADD()
{
    uint16_t regs;
    //readInstObj(&regs, sizeof(uint16_t));
    READ_OBJ(regs, uint16_t);

    uint8_t dest = DEST_REG(regs); //(regs & 0x0F00) >> 8;
    uint8_t left = LEFT_REG(regs); //(regs & 0x00F0) >> 4;
    uint8_t right = RIGHT_REG(regs); //regs & 0x000F;
    TRACE("%s,%s,%s", regToStr(dest), regToStr(left), regToStr(right));

    _operation(registers[dest], registers[left], registers[right], +);

    return false;
}

static inline bool doSUB()
{
    uint16_t regs;
    //readInstObj(&regs, sizeof(uint16_t));
    READ_OBJ(regs, uint16_t);

    uint8_t dest = DEST_REG(regs); //(regs & 0x0F00) >> 8;
    uint8_t left = LEFT_REG(regs); //(regs & 0x00F0) >> 4;
    uint8_t right = RIGHT_REG(regs); //regs & 0x000F;
    TRACE("%s,%s,%s", regToStr(dest), regToStr(left), regToStr(right));

    _operation(registers[dest], registers[left], registers[right], -);

    return false;
}

static inline bool doMUL()
{
    uint16_t regs;
    //readInstObj(&regs, sizeof(uint16_t));
    READ_OBJ(regs, uint16_t);

    uint8_t dest = DEST_REG(regs); //(regs & 0x0F00) >> 8;
    uint8_t left = LEFT_REG(regs); //(regs & 0x00F0) >> 4;
    uint8_t right = RIGHT_REG(regs); //regs & 0x000F;
    TRACE("%s,%s,%s", regToStr(dest), regToStr(left), regToStr(right));

    _operation(registers[dest], registers[left], registers[right], *);

    return false;
}

static inline bool doDIV()
{
    uint16_t regs;
    //readInstObj(&regs, sizeof(uint16_t));
    READ_OBJ(regs, uint16_t);

    uint8_t dest = DEST_REG(regs); //(regs & 0x0F00) >> 8;
    uint8_t left = LEFT_REG(regs); //(regs & 0x00F0) >> 4;
    uint8_t right = RIGHT_REG(regs); //regs & 0x000F;
    TRACE("%s,%s,%s", regToStr(dest), regToStr(left), regToStr(right));

    _div_by_zero(registers[right]);
    _operation(registers[dest], registers[left], registers[right], /);

    return false;
}

static inline bool doMOD()
{
    uint16_t regs;
    //readInstObj(&regs, sizeof(uint16_t));
    READ_OBJ(regs, uint16_t);

    uint8_t dest = DEST_REG(regs); //(regs & 0x0F00) >> 8;
    uint8_t left = LEFT_REG(regs); //(regs & 0x00F0) >> 4;
    uint8_t right = RIGHT_REG(regs); //regs & 0x000F;
    TRACE("%s,%s,%s", regToStr(dest), regToStr(left), regToStr(right));

    // Value* d = &registers[dest];
    // Value* l = &registers[left];
    // Value* r = &registers[right];

    _div_by_zero(registers[right]); //r);

    switch(registers[left].type) {
        case INT:
            switch(registers[right].type) {
                case INT:
                    registers[dest].type = INT;
                    registers[dest].data.num = registers[left].data.num % registers[right].data.num;
                    break;
                case UINT:
                    registers[dest].type = INT;
                    registers[dest].data.num = registers[left].data.num % registers[right].data.unum;
                    break;
                case FLOAT:
                    registers[dest].type = FLOAT;
                    registers[dest].data.fnum = fmod((double)registers[left].data.num, registers[right].data.fnum);
                    break;
                case BOOL:
                case STRING:
                case ERROR:
                    runtimeError("runtime error: cannot perform a '%%' on a %s", valTypeToStr(registers[right].type));
                    break;
                default:
                    fatalError("fatal error: unknown register type: %d", registers[right].type);
            }
            break;
        case UINT:
            switch(registers[right].type) {
                case INT:
                    registers[dest].type = INT;
                    registers[dest].data.num = registers[left].data.unum % registers[right].data.num;
                    break;
                case UINT:
                    registers[dest].type = UINT;
                    registers[dest].data.unum = registers[left].data.unum % registers[right].data.unum;
                    break;
                case FLOAT:
                    registers[dest].type = FLOAT;
                    registers[dest].data.fnum = fmod((double)registers[left].data.unum, registers[right].data.fnum);
                    break;
                case BOOL:
                case STRING:
                case ERROR:
                    runtimeError("runtime error: cannot perform a '%%' on a %s", valTypeToStr(registers[right].type));
                    break;
                default:
                    fatalError("fatal error: unknown register type: %d", registers[right].type);
            }
            break;
        case FLOAT:
            switch(registers[right].type) {
                case INT:
                    registers[dest].type = FLOAT;
                    registers[dest].data.fnum = fmod(registers[left].data.fnum, (double)registers[right].data.num);
                    break;
                case UINT:
                    registers[dest].type = FLOAT;
                    registers[dest].data.fnum = fmod(registers[left].data.fnum, (double)registers[right].data.unum);
                    break;
                case FLOAT:
                    registers[dest].type = FLOAT;
                    registers[dest].data.fnum = fmod(registers[left].data.fnum, registers[right].data.fnum);
                    break;
                case BOOL:
                case STRING:
                case ERROR:
                    runtimeError("runtime error: cannot perform a '%%' on a %s", valTypeToStr(registers[right].type));
                    break;
                default:
                    fatalError("fatal error: unknown register type: %d", registers[right].type);
            }
            break;
        case BOOL:
        case STRING:
        case ERROR:
            runtimeError("runtime error: cannot perform a '%%' on a %s", valTypeToStr(registers[left].type));
            break;
        default:
            fatalError("fatal error: unknown register type: %d", registers[left].type);
    }

    return false;
}

