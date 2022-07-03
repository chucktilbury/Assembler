
#include "gas.h"

typedef struct {
    Value** buf;
    uint32_t cap;
    uint32_t len;
} ValueBuf;

static ValueBuf vbuf;

void initValBuf()
{
    vbuf.cap = 0x01 << 3;
    vbuf.len = 0;
    vbuf.buf = _alloc_ds_array(Value*, vbuf.cap);
}

void saveValBuf(FILE* fp)
{
    fwrite(&vbuf.len, sizeof(vbuf.len), 1, fp);
    for(uint32_t idx = 0; idx < vbuf.len; idx++)
        fwrite(vbuf.buf[idx], sizeof(Value), 1, fp);
}

ValIdx addValBuf(Value* val)
{
    if(vbuf.len+1 > vbuf.cap) {
        vbuf.cap <<= 1;
        vbuf.buf = _realloc_ds_array(vbuf.buf, Value*, vbuf.cap);
    }

    // optomizers work pretty well...
    vbuf.buf[vbuf.len] = val;
    uint32_t idx = vbuf.len;
    vbuf.len++;
    return idx;
}

Value* getValBuf(ValIdx idx)
{
    if(idx < vbuf.len)
        return vbuf.buf[idx];
    else
        return NULL;
}

void dumpValBuf()
{
    printf("\n------- Dump Value Table -------\n");
    for(uint32_t idx = 0; idx < vbuf.len; idx++) {
        printf("    idx: %d ", idx);
        printVal(vbuf.buf[idx]);
        printf("\n");
    }
    printf("------- End of dump -------\n");
}

void printVal(Value* val)
{
    if(val != NULL) {
        printf("(%s)", valTypeToStr(val->type));
        switch(val->type) {
            case INT:
                printf("%ld", val->data.num);
                break;
            case UINT:
                printf("0x%lX", val->data.unum);
                break;
            case FLOAT:
                printf("%0.3f", val->data.fnum);
                break;
            case BOOL:
                printf("%s", val->data.bval? "true": "false");
                break;
            case STRING:
                printf("\"%s\"", val->data.str);
                break;
            default:
                printf("unknown type value: %d\n", val->type);
                return;
        }
    }
    else
        printf("<NONE>");
}

Value* createValue(ValType type)
{
    Value* val = _alloc_ds(Value);
    val->type = type;

    return val;
}

// TODO: Implement this
Value* castVal(ValType type, Value* val)
{
    val->type = type;
    return val;
}

const char* valTypeToStr(ValType type)
{
    return (type == INT)? "INT":
            (type == UINT)? "UINT":
            (type == FLOAT)? "FLOAT":
            (type == BOOL)? "BOOL":
            (type == STRING)? "STRING": "UNKNOWN";
}

