
#include "vmachine.h"

typedef struct {
    Value* list;
    uint32_t cap;
    uint32_t top;
} ValStack;

static ValStack vstack;

void initValStack()
{
    vstack.cap = 0x01 << 3;
    vstack.top = 0;
    vstack.list = _alloc_ds_array(Value, vstack.cap);
}

// return true if the stack index is valid
void validateStackIdx(ValIdx idx)
{
    // unsigned compare
    if((vstack.top-1)-idx > vstack.cap) {
        fprintf(stderr, "value stack under run\n");
        exit(1);
    }
}

void pushVal(Value val)
{
    if(vstack.top+1 > vstack.cap) {
        vstack.cap <<= 1;
        vstack.list = _realloc_ds_array(vstack.list, Value, vstack.cap);
    }

    vstack.list[vstack.top] = val;
    vstack.top++;
}

Value popVal()
{
    // unsigned wraps around
    if(vstack.top-1 > vstack.cap && vstack.top > 0) {
        fprintf(stderr, "value stack under run\n");
        exit(1);
    }

    vstack.top--;
    return vstack.list[vstack.top];
}

Value peekVal(ValIdx idx)
{
    validateStackIdx(idx);
//fprintf(stderr, "\nindex: %d", idx);
    return vstack.list[idx];
}

ValIdx getStackIdx()
{
    return vstack.top;
}

void dumpStack()
{
    fprintf(stderr, "\n");
    for(uint32_t i = 0; i < vstack.top; i++) {
        Value val = vstack.list[i];
        fprintf(stderr, "\n   %d. (%s)%s", i, valTypeToStr(val.type), valToStr(&val));
    }
}