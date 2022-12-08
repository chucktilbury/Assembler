
#include "common.h"

typedef struct {
    Value** buf;
    uint32_t cap;
    uint32_t len;
} ValueTab;

static ValueTab vbuf;

void initValTab() {
    vbuf.cap = 0x01 << 3;
    vbuf.len = 0;
    vbuf.buf = _alloc_ds_array(Value*, vbuf.cap);
}

void loadValTab(FILE* fp) {
    loadStrTab(fp);

    vbuf.cap = 1;
    vbuf.len = 0;
    fread(&vbuf.len, sizeof(vbuf.len), 1, fp);

    while(vbuf.len + 1 > vbuf.cap)
        vbuf.cap <<= 1;
    vbuf.buf = _alloc_ds_array(Value*, vbuf.cap);

    for(Index idx = 0; idx < vbuf.len; idx++) {
        vbuf.buf[idx] = _alloc(sizeof(Value));
        fread(vbuf.buf[idx], sizeof(Value), 1, fp);
        if(vbuf.buf[idx]->type == STRING)
            vbuf.buf[idx]->data.str = getStr(vbuf.buf[idx]->data.stridx);
    }
}

void saveValTab(FILE* fp) {
    initStrTab();

    for(uint32_t idx = 0; idx < vbuf.len; idx++) {
        if(vbuf.buf[idx]->type == STRING) {
            if(vbuf.buf[idx]->isAssigned && vbuf.buf[idx]->data.str != NULL)
                vbuf.buf[idx]->data.stridx = addStr(vbuf.buf[idx]->data.str);
        }
    }
    saveStrTab(fp);

    fwrite(&vbuf.len, sizeof(vbuf.len), 1, fp);
    for(uint32_t idx = 0; idx < vbuf.len; idx++)
        fwrite(vbuf.buf[idx], sizeof(Value), 1, fp);
}

ValIdx addValTab(Value* val) {
    if(vbuf.len + 1 > vbuf.cap) {
        vbuf.cap <<= 1;
        vbuf.buf = _realloc_ds_array(vbuf.buf, Value*, vbuf.cap);
    }

    // optimizers work pretty well...
    vbuf.buf[vbuf.len] = val;
    uint32_t idx = vbuf.len;
    vbuf.len++;
    return idx;
}

Value* getValTab(ValIdx idx) {
    if(idx < vbuf.len) {
        // for(unsigned i = 0; i < vbuf.len; i++)
        //     fprintf(stderr, "%d. %p\n", i, vbuf.buf[i]);
        return vbuf.buf[idx];
    }
    else
        return NULL;
}

void dumpValTab() {
    printf("\n------- Dump Value Table -------\n");
    for(uint32_t idx = 0; idx < vbuf.len; idx++) {
        printf("    idx: %d %s\n", idx, valToStr(vbuf.buf[idx]));
    }
    printf("------- End of dump -------\n");
}

void printVal(Value* val) {
    if(val != NULL) {
        // printf("(%s)", valTypeToStr(val->type));
        switch(val->type) {
            // case INT:
            //     printf("%ld", val->data.num);
            //     break;
            // case UINT:
            //     printf("0x%lX", val->data.unum);
            //     break;
            // case FLOAT:
            //     printf("%0.5f", val->data.fnum);
            //     break;
            case NUM: printf("%0.5f", val->data.num); break;
            case BOOL: printf("%s", val->data.bval ? "true" : "false"); break;
            case STRING:
                // printf("%s", getStr(val->data.str));
                printf("%s", val->data.str);
                break;
            default: printf("unknown type value: %d\n", val->type); return;
        }
    }
    else
        printf("<NONE>");
}

Value* createValue(ValType type) {
    Value* val = _alloc_ds(Value);
    val->type = type;
    val->isAssigned = false;

    return val;
}

// TODO: Implement this
Value* castVal(ValType type, Value* val) {
    if(val != NULL) {
        switch(type) {
            case NUM:
                switch(val->type) {
                    case NUM: break;
                    case BOOL:
                        val->data.num = val->data.bval ? 1.0 : 0.0;
                        break;
                    case STRING:
                        // val->data.fnum = strtod(getStr(val->data.str), NULL);
                        val->data.num = strtod(val->data.str, NULL);
                        break;
                    default:
                        fprintf(stderr, "fatal error: unknown type value: %d\n",
                                val->type);
                        exit(1);
                }
                break;
            case BOOL:
                switch(val->type) {
                    case NUM:
                        val->data.bval = val->data.num != 0.0 ? true : false;
                        break;
                    case BOOL: break;
                    case STRING:
                        // val->data.bval = getStr(val->data.str) == NULL? false: true;
                        val->data.bval = val->data.str == NULL ? false : true;
                        break;
                    default:
                        fprintf(stderr, "fatal error: unknown type value: %d\n",
                                val->type);
                        exit(1);
                }
                break;
            case STRING:
                switch(val->type) {
                    case NUM:
                    case BOOL:
                        fprintf(stderr, "syntax: cannot convert a %s to a STRING. Use format instead.",
                                valTypeToStr(val->type));
                        exit(1);
                        break;
                    case STRING: break;
                    default:
                        fprintf(stderr, "fatal error: unknown type value: %d\n",
                                val->type);
                        exit(1);
                }
                break;
            default:
                fprintf(stderr, "fatal error: unknown type value: %d\n", type);
                exit(1);
        }
        val->type = type;
    }
    return val;
}

const char* valTypeToStr(ValType type) {
    return (type == NUM) ? "NUMBER" :
    (type == BOOL)       ? "BOOL" :
    (type == UDATA)      ? "UDATA" :
    (type == STRING)     ? "STRING" :
                           "UNKNOWN";
}

void assignVal(ValIdx idx, Value* val) {
    if(idx < vbuf.len)
        memcpy(vbuf.buf[idx], val, sizeof(Value));
}

const char* valToStr(Value* val) {
    char buf[60];
    if(val->isAssigned) {
        switch(val->type) {
            case NUM: snprintf(buf, sizeof(buf), "%0.4f", val->data.num); break;
            case BOOL:
                snprintf(buf, sizeof(buf), "%s", val->data.bval ? "true" : "false");
                break;
            case STRING:
                // snprintf(buf, sizeof(buf), "%s", getStr(val->data.str));
                snprintf(buf, sizeof(buf), "%s", val->data.str);
                break;
            default: snprintf(buf, sizeof(buf), "unknown"); break;
        }
    }
    else
        strcpy(buf, "unassigned");

    return _copy_str(buf);
}
