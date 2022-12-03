
#include "vmachine.h"
#include <math.h>

extern Value registers[16];
extern int errors;

#define _div_by_zero(r) do { \
        switch((r).type) { \
            case NUM: \
                if((r).data.num == 0.0) { \
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
            case NUM: \
                switch((r).type) { \
                    case NUM: \
                        (d).type = NUM; \
                        (d).data.num = (l).data.num op (r).data.num; \
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
    READ_OBJ(regs, uint8_t);

    uint8_t dest = RIGHT_REG(regs);
    uint8_t reg = LEFT_REG(regs);
    TRACE("%s,%s", regToStr(dest), regToStr(reg));

    registers[dest].type = registers[reg].type;
    switch(registers[reg].type) {
        case NUM: registers[dest].data.num = -registers[reg].data.num; break;
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
    READ_OBJ(regs, uint16_t);

    uint8_t dest = DEST_REG(regs);
    uint8_t left = LEFT_REG(regs);
    uint8_t right = RIGHT_REG(regs);
    TRACE("%s,%s,%s", regToStr(dest), regToStr(left), regToStr(right));

    _operation(registers[dest], registers[left], registers[right], +);

    return false;
}

static inline bool doSUB()
{
    uint16_t regs;
    READ_OBJ(regs, uint16_t);

    uint8_t dest = DEST_REG(regs);
    uint8_t left = LEFT_REG(regs);
    uint8_t right = RIGHT_REG(regs);
    TRACE("%s,%s,%s", regToStr(dest), regToStr(left), regToStr(right));

    _operation(registers[dest], registers[left], registers[right], -);

    return false;
}

static inline bool doMUL()
{
    uint16_t regs;
    READ_OBJ(regs, uint16_t);

    uint8_t dest = DEST_REG(regs);
    uint8_t left = LEFT_REG(regs);
    uint8_t right = RIGHT_REG(regs);
    TRACE("%s,%s,%s", regToStr(dest), regToStr(left), regToStr(right));

    _operation(registers[dest], registers[left], registers[right], *);

    return false;
}

static inline bool doDIV()
{
    uint16_t regs;
    READ_OBJ(regs, uint16_t);

    uint8_t dest = DEST_REG(regs);
    uint8_t left = LEFT_REG(regs);
    uint8_t right = RIGHT_REG(regs);
    TRACE("%s,%s,%s", regToStr(dest), regToStr(left), regToStr(right));

    _div_by_zero(registers[right]);
    _operation(registers[dest], registers[left], registers[right], /);

    return false;
}

static inline bool doMOD()
{
    uint16_t regs;
    READ_OBJ(regs, uint16_t);

    uint8_t dest = DEST_REG(regs);
    uint8_t left = LEFT_REG(regs);
    uint8_t right = RIGHT_REG(regs);
    TRACE("%s,%s,%s", regToStr(dest), regToStr(left), regToStr(right));

    _div_by_zero(registers[right]); //r);

    switch(registers[left].type) {
        case NUM:
            switch(registers[right].type) {
                case NUM:
                    registers[dest].type = NUM;
                    registers[dest].data.num = fmod(registers[left].data.num, registers[right].data.num);
                    break;
                case BOOL:
                case STRING:
                case ERROR:
                    runtimeError("cannot perform a '%%' on a %s", valTypeToStr(registers[right].type));
                    break;
                default:
                    fatalError("unknown register type: %d", registers[right].type);
            }
            break;
        case BOOL:
        case STRING:
        case ERROR:
            runtimeError("cannot perform a '%%' on a %s", valTypeToStr(registers[left].type));
            break;
        default:
            fatalError("unknown register type: %d", registers[left].type);
    }

    return false;
}

