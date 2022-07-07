
#include "vmachine.h"

extern Value registers[16];

#define _operation(l, r, op) do { \
        switch((l)->type) { \
            case ERROR: \
            case STRING: \
                switch((r)->type) { \
                    case ERROR: \
                    case STRING: \
                        setZflag(true op true); \
                        break; \
                    case INT: \
                        setZflag(true op ((r)->data.num != 0)); \
                        break; \
                    case UINT: \
                        setZflag(true op ((r)->data.unum != 0)); \
                        break; \
                    case FLOAT: \
                        setZflag(true op ((r)->data.fnum != 0.0)); \
                        break; \
                    case BOOL: \
                        setZflag(true op (r)->data.bval); \
                        break; \
                    default: \
                        fatalError("internal error: invalid right var type"); \
                } \
                break; \
             case INT: \
                switch((r)->type) { \
                    case ERROR: \
                    case STRING: \
                        setZflag(((l)->data.num != 0) op true); \
                        break; \
                    case INT: \
                        setZflag((l)->data.num op (r)->data.num); \
                        break; \
                    case UINT: \
                        setZflag((l)->data.num op (int64_t)(r)->data.unum); \
                        break; \
                    case FLOAT: \
                        setZflag((l)->data.num op (r)->data.fnum); \
                        break; \
                    case BOOL: \
                        setZflag(((l)->data.num != 0) op (r)->data.bval); \
                        break; \
                    default: \
                        fatalError("internal error: invalid right var type"); \
                } \
                break; \
            case UINT: \
                switch((r)->type) { \
                    case ERROR: \
                    case STRING: \
                        setZflag(((r)->data.unum != 0) op true); \
                        break; \
                    case INT: \
                        setZflag((int64_t)(l)->data.unum op (r)->data.num); \
                        break; \
                    case UINT: \
                        setZflag((l)->data.unum op (r)->data.unum); \
                        break; \
                    case FLOAT: \
                        setZflag((double)(l)->data.unum op (r)->data.fnum); \
                        break; \
                    case BOOL: \
                        setZflag(((l)->data.unum != 0) op (r)->data.bval); \
                        break; \
                    default: \
                        fatalError("internal error: invalid right var type"); \
                } \
                break; \
            case FLOAT: \
                switch((r)->type) { \
                    case ERROR: \
                    case STRING: \
                        setZflag(((r)->data.fnum != 0.0) op true); \
                        break; \
                    case INT: \
                        setZflag((l)->data.fnum op (r)->data.num); \
                        break; \
                    case UINT: \
                        setZflag((l)->data.fnum op (r)->data.unum); \
                        break; \
                    case FLOAT: \
                        setZflag((l)->data.fnum op (r)->data.fnum); \
                        break; \
                    case BOOL: \
                        setZflag(((l)->data.fnum != 0.0) op (r)->data.bval); \
                        break; \
                    default: \
                        fatalError("internal error: invalid right var type"); \
                } \
                break; \
            case BOOL: \
                switch((r)->type) { \
                    case ERROR: \
                    case STRING: \
                        setZflag((l)->data.bval op true); \
                        break; \
                    case INT: \
                        setZflag((l)->data.bval op ((r)->data.num != 0)); \
                        break; \
                    case UINT: \
                        setZflag((l)->data.bval op ((r)->data.unum != 0)); \
                        break; \
                    case FLOAT: \
                        setZflag((l)->data.bval op ((r)->data.fnum != 0)); \
                        break; \
                    case BOOL: \
                        setZflag((l)->data.bval op (r)->data.bval); \
                        break; \
                    default: \
                        fatalError("internal error: invalid right var type"); \
                } \
                break; \
            default: \
                fatalError("internal error: invalid left var type"); \
        } \
    } while(false)

bool doNOT()
{
    uint8_t reg;
    readInstObj(&reg, sizeof(uint8_t));

    Value* val = &registers[reg & 0x0F];
    switch(val->type) {
        case ERROR:
        case STRING:
            setZflag(true); // always not zero, so not = true
            break;
        case INT:
            setZflag(val->data.num != 0);
            break;
        case UINT:
            setZflag(val->data.unum != 0);
            break;
        case FLOAT:
            setZflag(val->data.fnum != 0.0);
            break;
        case BOOL:
            setZflag(val->data.bval? false: true);
            break;
        default:
            fatalError("internal error: invalid var type in NOT"); \
    }

    return false;
}

// Some of these macro expansions are always true, but still correct.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbool-compare"

bool doEQ()
{
    uint8_t regs;
    readInstObj(&regs, sizeof(uint8_t));

    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    _operation(&registers[left], &registers[right], ==);

    return false;
}

bool doNEQ()
{
    uint8_t regs;
    readInstObj(&regs, sizeof(uint8_t));

    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    _operation(&registers[left], &registers[right], !=);

    return false;
}

bool doLEQ()
{
    uint8_t regs;
    readInstObj(&regs, sizeof(uint8_t));

    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    _operation(&registers[left], &registers[right], <=);

    return false;
}

bool doGEQ()
{
    uint8_t regs;
    readInstObj(&regs, sizeof(uint8_t));

    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    _operation(&registers[left], &registers[right], >=);

    return false;
}

bool doLESS()
{
    uint8_t regs;
    readInstObj(&regs, sizeof(uint8_t));

    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    _operation(&registers[left], &registers[right], <);

    return false;
}

bool doGTR()
{
    uint8_t regs;
    readInstObj(&regs, sizeof(uint8_t));

    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    _operation(&registers[left], &registers[right], >);

    return false;
}

#pragma GCC diagnostic pop
