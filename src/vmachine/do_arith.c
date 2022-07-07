
#include "vmachine.h"
#include <math.h>

extern Value registers[16];
extern int errors;

#define _div_by_zero(r) do { \
        switch((r)->type) { \
            case INT: \
                if((r)->data.num == 0) { \
                    fatalError("divide by zero"); \
                } \
                break; \
            case UINT: \
                if((r)->data.unum == 0) { \
                    fatalError("divide by zero"); \
                } \
                break; \
            case FLOAT: \
                if((r)->data.fnum == 0.0) { \
                    fatalError("divide by zero"); \
                } \
                break; \
            case BOOL: \
            case STRING: \
            case ERROR: \
                break; \
            default: \
                fatalError("fatal error: unknown register type: %d", (r)->type); \
        } \
    } while(false)


#define _operation(d, l, r, op) do { \
        switch((l)->type) { \
            case INT: \
                switch((r)->type) { \
                    case INT: \
                        (d)->type = INT; \
                        (d)->data.num = (l)->data.num op (r)->data.num; \
                        break; \
                    case UINT: \
                        (d)->type = INT; \
                        (d)->data.num = (l)->data.num op (r)->data.unum; \
                        break; \
                    case FLOAT: \
                        (d)->type = FLOAT; \
                        (d)->data.fnum = (l)->data.num op (r)->data.fnum; \
                        break; \
                    case BOOL: \
                    case STRING: \
                    case ERROR: \
                        runtimeError("runtime error: cannot perform a '%s' on a %s", #op, valTypeToStr((l)->type)); \
                        break; \
                    default: \
                        fatalError("fatal error: unknown register type: %d", (r)->type); \
                } \
                break; \
            case UINT: \
                switch((r)->type) { \
                    case INT: \
                        (d)->type = INT; \
                        (d)->data.num = (l)->data.unum op (r)->data.num; \
                        break; \
                    case UINT: \
                        (d)->type = UINT; \
                        (d)->data.unum = (l)->data.unum op (r)->data.unum; \
                        break; \
                    case FLOAT: \
                        (d)->type = FLOAT; \
                        (d)->data.fnum = (l)->data.unum op (r)->data.fnum; \
                        break; \
                    case BOOL: \
                    case STRING: \
                    case ERROR: \
                        runtimeError("runtime error: cannot perform a '%s' on a %s", #op, valTypeToStr((l)->type)); \
                        break; \
                    default: \
                        fatalError("fatal error: unknown register type: %d", (r)->type); \
                } \
                break; \
            case FLOAT: \
                switch((r)->type) { \
                    case INT: \
                        (d)->type = FLOAT; \
                        (d)->data.fnum = (l)->data.fnum op (r)->data.num; \
                        break; \
                    case UINT: \
                        (d)->type = FLOAT; \
                        (d)->data.fnum = (l)->data.fnum op (r)->data.unum; \
                        break; \
                    case FLOAT: \
                        (d)->type = FLOAT; \
                        (d)->data.fnum = (l)->data.fnum op (r)->data.fnum; \
                        break; \
                    case BOOL: \
                    case STRING: \
                    case ERROR: \
                        runtimeError("runtime error: cannot perform a '%s' on a %s", #op, valTypeToStr((l)->type)); \
                        break; \
                    default: \
                        fatalError("fatal error: unknown register type: %d", (r)->type); \
                } \
                break; \
            case BOOL: \
            case STRING: \
            case ERROR: \
                    runtimeError("runtime error: cannot perform a '%s' on a %s", #op, valTypeToStr((l)->type)); \
                errors++; \
                break; \
            default: \
                fatalError("fatal error: unknown register type: %d", (l)->type); \
        } \
    } while(false)

bool doNEG()
{
    uint8_t regs;
    readInstObj(&regs, sizeof(uint8_t));

    uint8_t dest = (regs & 0x00F0) >> 4;
    uint8_t reg = regs & 0x000F;

    Value* res = &registers[dest];
    Value* val = &registers[reg];

    res->type = val->type;
    switch(val->type) {
        case INT: res->data.num = -val->data.num; break;
        case UINT: res->data.unum = -val->data.unum; break;
        case FLOAT: res->data.fnum = -val->data.fnum; break;
        case BOOL: res->data.bval = val->data.bval? false: true; break;
        case STRING:
            res->type = ERROR;
            runtimeError("runtime error: cannot negate a string");
            break;
        case ERROR: /* do nothing */ break;
        default:
            fatalError("fatal error: unknown register type: %d", val->type);
    }

    return false;
}

bool doADD()
{
    uint16_t regs;
    readInstObj(&regs, sizeof(uint16_t));

    uint8_t dest = (regs & 0x0F00) >> 8;
    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    _operation(&registers[dest], &registers[left], &registers[right], +);

    return false;
}

bool doSUB()
{
    uint16_t regs;
    readInstObj(&regs, sizeof(uint16_t));

    uint8_t dest = (regs & 0x0F00) >> 8;
    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    _operation(&registers[dest], &registers[left], &registers[right], -);

    return false;
}

bool doMUL()
{
    uint16_t regs;
    readInstObj(&regs, sizeof(uint16_t));

    uint8_t dest = (regs & 0x0F00) >> 8;
    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    _operation(&registers[dest], &registers[left], &registers[right], *);

    return false;
}

bool doDIV()
{
    uint16_t regs;
    readInstObj(&regs, sizeof(uint16_t));

    uint8_t dest = (regs & 0x0F00) >> 8;
    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    _div_by_zero(&registers[right]);
    _operation(&registers[dest], &registers[left], &registers[right], /);

    return false;
}

bool doMOD()
{
    uint16_t regs;
    readInstObj(&regs, sizeof(uint16_t));

    uint8_t dest = (regs & 0x0F00) >> 8;
    uint8_t left = (regs & 0x00F0) >> 4;
    uint8_t right = regs & 0x000F;

    Value* d = &registers[dest];
    Value* l = &registers[left];
    Value* r = &registers[right];

    _div_by_zero(r);

    switch(l->type) {
        case INT:
            switch(r->type) {
                case INT:
                    d->type = INT;
                    d->data.num = l->data.num % r->data.num;
                    break;
                case UINT:
                    d->type = INT;
                    d->data.num = l->data.num % r->data.unum;
                    break;
                case FLOAT:
                    d->type = FLOAT;
                    d->data.fnum = fmod((double)l->data.num, r->data.fnum);
                    break;
                case BOOL:
                case STRING:
                case ERROR:
                    runtimeError("runtime error: cannot perform a '%%' on a %s", valTypeToStr(r->type));
                    break;
                default:
                    fatalError("fatal error: unknown register type: %d", r->type);
            }
            break;
        case UINT:
            switch(r->type) {
                case INT:
                    d->type = INT;
                    d->data.num = l->data.unum % r->data.num;
                    break;
                case UINT:
                    d->type = UINT;
                    d->data.unum = l->data.unum % r->data.unum;
                    break;
                case FLOAT:
                    d->type = FLOAT;
                    d->data.fnum = fmod((double)l->data.unum, r->data.fnum);
                    break;
                case BOOL:
                case STRING:
                case ERROR:
                    runtimeError("runtime error: cannot perform a '%%' on a %s", valTypeToStr(r->type));
                    break;
                default:
                    fatalError("fatal error: unknown register type: %d", r->type);
            }
            break;
        case FLOAT:
            switch(r->type) {
                case INT:
                    d->type = FLOAT;
                    d->data.fnum = fmod(l->data.fnum, (double)r->data.num);
                    break;
                case UINT:
                    d->type = FLOAT;
                    d->data.fnum = fmod(l->data.fnum, (double)r->data.unum);
                    break;
                case FLOAT:
                    d->type = FLOAT;
                    d->data.fnum = fmod(l->data.fnum, r->data.fnum);
                    break;
                case BOOL:
                case STRING:
                case ERROR:
                    runtimeError("runtime error: cannot perform a '%%' on a %s", valTypeToStr(r->type));
                    break;
                default:
                    fatalError("fatal error: unknown register type: %d", r->type);
            }
            break;
        case BOOL:
        case STRING:
        case ERROR:
            runtimeError("runtime error: cannot perform a '%%' on a %s", valTypeToStr(l->type));
            break;
        default:
            fatalError("fatal error: unknown register type: %d", l->type);
    }

    return false;
}

