
#include "common.h"

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

void loadValBuf(FILE* fp)
{
    vbuf.cap = 1;
    vbuf.len = 0;
    fread(&vbuf.len, sizeof(vbuf.len), 1, fp);

    while(vbuf.len+1 > vbuf.cap)
        vbuf.cap <<= 1;
    vbuf.buf = _alloc_ds_array(Value*, vbuf.cap);

    for(Index idx = 0; idx < vbuf.len; idx++) {
        vbuf.buf[idx] = _alloc(sizeof(Value));
        fread(vbuf.buf[idx], sizeof(Value), 1, fp);
    }
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

    // optimizers work pretty well...
    vbuf.buf[vbuf.len] = val;
    uint32_t idx = vbuf.len;
    vbuf.len++;
    return idx;
}

Value* getValBuf(ValIdx idx)
{
    if(idx < vbuf.len) {
        // for(unsigned i = 0; i < vbuf.len; i++)
        //     fprintf(stderr, "%d. %p\n", i, vbuf.buf[i]);
        return vbuf.buf[idx];
    }
    else
        return NULL;
}

void dumpValBuf()
{
    printf("\n------- Dump Value Table -------\n");
    for(uint32_t idx = 0; idx < vbuf.len; idx++) {
        printf("    idx: %d %s\n", idx, valToStr(vbuf.buf[idx]));
    }
    printf("------- End of dump -------\n");
}

void printVal(Value* val)
{
    if(val != NULL) {
        //printf("(%s)", valTypeToStr(val->type));
        switch(val->type) {
            case INT:
                printf("%ld", val->data.num);
                break;
            case UINT:
                printf("0x%lX", val->data.unum);
                break;
            case FLOAT:
                printf("%0.5f", val->data.fnum);
                break;
            case BOOL:
                printf("%s", val->data.bval? "true": "false");
                break;
            case STRING:
                //printf("idx: %d\t\"%s\"", val->data.str, getStr(val->data.str));
                printf("%s", getStr(val->data.str));
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
    val->isAssigned = false;

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

void assignVal(ValIdx idx, Value* val)
{
    if(idx < vbuf.len)
        memcpy(vbuf.buf[idx], val, sizeof(Value));
}

const char* valToStr(Value* val)
{
    char buf[60];
    //sprintf(buf, "(%s)", valTypeToStr(val->type));
    //int len = strlen(buf);
    switch(val->type) {
        case INT:
            //snprintf(&buf[len], sizeof(buf)-len, "%ld", val->data.num);
            snprintf(buf, sizeof(buf), "%ld", val->data.num);
            break;
        case UINT:
            //snprintf(&buf[len], sizeof(buf)-len, "0x%lX", val->data.unum);
            snprintf(buf, sizeof(buf), "0x%lX", val->data.unum);
            break;
        case FLOAT:
            //snprintf(&buf[len], sizeof(buf)-len, "%0.3f", val->data.fnum);
            snprintf(buf, sizeof(buf), "%0.4f", val->data.fnum);
            break;
        case BOOL:
            //snprintf(&buf[len], sizeof(buf)-len, "%s", val->data.bval? "true": "false");
            snprintf(buf, sizeof(buf), "%s", val->data.bval? "true": "false");
            break;
        case STRING:
            //snprintf(&buf[len], sizeof(buf)-len, "(%d)%s", val->data.str, getStr(val->data.str));
            snprintf(buf, sizeof(buf), "(%d)%s", val->data.str, getStr(val->data.str));
            break;
        default:
            //snprintf(&buf[len], sizeof(buf)-len, "unknown");
            snprintf(buf, sizeof(buf), "unknown");
            break;
    }

    return _copy_str(buf);
}