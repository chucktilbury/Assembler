
#include "vmachine.h"
#include <math.h>

extern Value registers[16];
extern int errors;

#define _div_by_zero(r) do { \
        switch((r)->type) { \
            case INT: \
                if((r)->data.num == 0) { \
                    fprintf(stderr, "fatal error: divide by zero"); \
                    exit(1); \
                } \
                break; \
            case UINT: \
                if((r)->data.unum == 0) { \
                    fprintf(stderr, "fatal error: divide by zero"); \
                    exit(1); \
                } \
                break; \
            case FLOAT: \
                if((r)->data.fnum == 0.0) { \
                    fprintf(stderr, "fatal error: divide by zero"); \
                    exit(1); \
                } \
                break; \
            case BOOL: \
            case STRING: \
            case ERROR: \
                break; \
            default: \
                fprintf(stderr, "fatal error: unknown register type: %d\n", (r)->type); \
                exit(1); \
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
                        fprintf(stderr, "runtime error: cannot perform a '%s' on a %s", \
                                #op, valTypeToStr((l)->type)); \
                        errors++; \
                        break; \
                    default: \
                        fprintf(stderr, "fatal error: unknown register type: %d\n", (r)->type); \
                        exit(1); \
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
                        fprintf(stderr, "runtime error: cannot perform a '%s' on a %s", \
                                #op, valTypeToStr((l)->type)); \
                        errors++; \
                        break; \
                    default: \
                        fprintf(stderr, "fatal error: unknown register type: %d\n", (r)->type); \
                        exit(1); \
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
                        fprintf(stderr, "runtime error: cannot perform a '%s' on a %s", \
                                #op, valTypeToStr((l)->type)); \
                        errors++; \
                        break; \
                    default: \
                        fprintf(stderr, "fatal error: unknown register type: %d\n", (r)->type); \
                        exit(1); \
                } \
                break; \
            case BOOL: \
            case STRING: \
            case ERROR: \
                fprintf(stderr, "runtime error: cannot perform a '%s' on a %s", \
                        #op, valTypeToStr((l)->type)); \
                errors++; \
                break; \
            default: \
                fprintf(stderr, "fatal error: unknown register type: %d\n", (l)->type); \
                exit(1); \
        } \
    } while(false)

void doNEG(uint8_t dest, uint8_t reg)
{
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
            fprintf(stderr, "runtime error: cannot negate a string\n");
            errors++;
            break;
        case ERROR: /* do nothing */ break;
        default:
            fprintf(stderr, "fatal error: unknown register type: %d\n", val->type);
            exit(1);
    }
}

void doADD(uint8_t dest, uint8_t left, uint8_t right)
{
    _operation(&registers[dest], &registers[left], &registers[right], +);
}

void doSUB(uint8_t dest, uint8_t left, uint8_t right)
{
    _operation(&registers[dest], &registers[left], &registers[right], -);
}

void doMUL(uint8_t dest, uint8_t left, uint8_t right)
{
    _operation(&registers[dest], &registers[left], &registers[right], *);
}

void doDIV(uint8_t dest, uint8_t left, uint8_t right)
{
    _div_by_zero(&registers[right]);
    _operation(&registers[dest], &registers[left], &registers[right], /);
}

void doMOD(uint8_t dest, uint8_t left, uint8_t right)
{
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
                    fprintf(stderr, "runtime error: cannot perform a '%%' on a %s",
                            valTypeToStr(l->type));
                    errors++;
                    break;
                default:
                    fprintf(stderr, "fatal error: unknown register type: %dn", r->type);
                    exit(1);
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
                    fprintf(stderr, "runtime error: cannot perform a '%%' on a %s",
                            valTypeToStr(l->type));
                    errors++;
                    break;
                default:
                    fprintf(stderr, "fatal error: unknown register type: %dn", r->type);
                    exit(1);
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
                    fprintf(stderr, "runtime error: cannot perform a '%%' on a %s",
                            valTypeToStr(l->type));
                    errors++;
                    break;
                default:
                    fprintf(stderr, "fatal error: unknown register type: %dn", r->type);
                    exit(1);
            }
            break;
        case BOOL:
        case STRING:
        case ERROR:
            fprintf(stderr, "runtime error: cannot perform a '%%' on a %s",
                    valTypeToStr(l->type));
            errors++;
            break;
        default:
            fprintf(stderr, "fatal error: unknown register type: %dn", l->type);
            exit(1);
    }

}

