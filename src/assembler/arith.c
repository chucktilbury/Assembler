
#include "gas.h"

#define _operation(dest, left, right, op)                                     \
    do {                                                                      \
        dest->isAssigned = true;                                              \
        dest->isConst = true;                                                 \
        switch(left->type) {                                                  \
            case NUM:                                                         \
                switch(right->type) {                                         \
                    case NUM:                                                 \
                        dest->type = NUM;                                     \
                        dest->data.num = left->data.num op right->data.num;   \
                        break;                                                \
                    case BOOL:                                                \
                    case STRING:                                              \
                        syntaxError(                                          \
                        "invalid right operand to constant expression");      \
                        break;                                                \
                    default:                                                  \
                        fatalError(                                           \
                        "invalid right operand type in constant expression"); \
                }                                                             \
                break;                                                        \
            case BOOL:                                                        \
            case STRING:                                                      \
                syntaxError("invalid left operand to constant expression");   \
                break;                                                        \
            default:                                                          \
                fatalError(                                                   \
                "invalid left operand type in constant expression");          \
        }                                                                     \
    } while(false)

void addNum(Value* dest, Value* left, Value* right) {
    _operation(dest, left, right, +);
}

void subNum(Value* dest, Value* left, Value* right) {
    _operation(dest, left, right, -);
}

void mulNum(Value* dest, Value* left, Value* right) {
    _operation(dest, left, right, *);
}

void divNum(Value* dest, Value* left, Value* right) {
    switch(right->type) {
        case NUM:
            if(right->data.num == 0.0)
                syntaxError("divide by zero in constant expression");
            break;
        case BOOL:
        case STRING:
            syntaxError("invalid operand to constant expression");
            break;
        default: fatalError("invalid operand type in constant expression");
    }

    _operation(dest, left, right, /);
}

void negNum(Value* dest, Value* val) {
    dest->isAssigned = true;
    dest->isConst = true;

    switch(val->type) {
        case NUM:
            dest->type = NUM;
            dest->data.num = -val->data.num;
            break;
        case BOOL:
        case STRING:
            syntaxError("invalid operand to constant expression");
            break;
        default: fatalError("invalid operand type in constant expression");
    }
}
