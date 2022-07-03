
#include "gas.h"

#define _operation(dest, left, right, op) do { \
    dest->isAssigned = true; \
    dest->isConst = true; \
    switch(left->type) { \
        case INT: \
            switch(right->type) { \
                case INT: \
                    dest->type = INT; \
                    dest->data.num = left->data.num op right->data.num; \
                    break; \
                case UINT: \
                    dest->type = INT; \
                    dest->data.num = left->data.num op right->data.unum; \
                    break; \
                case FLOAT: \
                    dest->type = FLOAT; \
                    dest->data.fnum = left->data.num op right->data.fnum; \
                    break; \
                case BOOL: \
                case STRING: \
                    syntaxError("invalid right operand to constant expression"); \
                    break; \
                default: \
                    fatalError("invalid right operand type in constant expression"); \
            } \
            break; \
        case UINT: \
            switch(right->type) { \
                case INT: \
                    dest->type = INT; \
                    dest->data.num = left->data.unum op right->data.num; \
                    break; \
                case UINT: \
                    dest->type = UINT; \
                    dest->data.unum = left->data.unum op right->data.unum; \
                    break; \
                case FLOAT: \
                    dest->type = FLOAT; \
                    dest->data.fnum = left->data.unum op right->data.fnum; \
                    break; \
                case BOOL: \
                case STRING: \
                    syntaxError("invalid right operand to constant expression"); \
                    break; \
                default: \
                    fatalError("invalid right operand type in constant expression"); \
            } \
            break; \
        case FLOAT: \
            switch(right->type) { \
                case INT: \
                    dest->type = FLOAT; \
                    dest->data.fnum = left->data.fnum op right->data.num; \
                    break; \
                case UINT: \
                    dest->type = FLOAT; \
                    dest->data.fnum = left->data.fnum op right->data.unum; \
                    break; \
                case FLOAT: \
                    dest->type = FLOAT; \
                    dest->data.fnum = left->data.fnum op right->data.fnum; \
                    break; \
                case BOOL: \
                case STRING: \
                    syntaxError("invalid right operand to constant expression"); \
                    break; \
                default: \
                    fatalError("invalid right operand type in constant expression"); \
            } \
            break; \
        case BOOL: \
        case STRING: \
            syntaxError("invalid left operand to constant expression"); \
            break; \
        default: \
            fatalError("invalid left operand type in constant expression"); \
    } \
} while(false) \

void addNum(Value* dest, Value* left, Value* right)
{
    _operation(dest, left, right, +);
}

void subNum(Value* dest, Value* left, Value* right)
{
    _operation(dest, left, right, -);
}

void mulNum(Value* dest, Value* left, Value* right)
{
    _operation(dest, left, right, *);
}

void divNum(Value* dest, Value* left, Value* right)
{
    switch(right->type) {
        case INT:
            if(right->data.num == 0)
                syntaxError("divide by zero in constant expression");
            break;
        case UINT:
            if(right->data.unum == 0)
                syntaxError("divide by zero in constant expression");
            break;
        case FLOAT:
            if(right->data.fnum == 0)
                syntaxError("divide by zero in constant expression");
            break;
        case BOOL:
        case STRING:
            syntaxError("invalid operand to constant expression");
            break;
        default:
            fatalError("invalid operand type in constant expression");
    }

    _operation(dest, left, right, /);
}

void negNum(Value* dest, Value* val)
{
    dest->isAssigned = true;
    dest->isConst = true;

    switch(val->type) {
        case INT:
            dest->type = INT;
            dest->data.num = -val->data.num;
            break;
        case UINT:
            dest->type = UINT;
            dest->data.unum = (uint64_t)(-((int64_t)val->data.num));
            break;
        case FLOAT:
            dest->type = FLOAT;
            dest->data.fnum = -val->data.fnum;
            break;
        case BOOL:
        case STRING:
            syntaxError("invalid operand to constant expression");
            break;
        default:
            fatalError("invalid operand type in constant expression");
    }
}
