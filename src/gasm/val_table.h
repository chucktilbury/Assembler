#ifndef VAL_BUFFER_H
#define VAL_BUFFER_H

typedef enum {
    ERROR,
    INT,
    UINT,
    FLOAT,
    BOOL,
    STRING,
} ValType;

typedef uint32_t ValIdx;

typedef struct {
    ValType type;
    bool isAssigned;
    bool isConst;
    int references;
    union {
        uint64_t unum;
        int64_t num;
        double fnum;
        bool bval;
        StrIdx str;
    } data;
} Value;

void initValBuf();
void saveValBuf(FILE* fp);
ValIdx addValBuf(Value* val);
Value* getValBuf(ValIdx idx);
void dumpValBuf();
void printVal(Value* val);
Value* createValue(ValType type);
Value* castVal(ValType type, Value* val);
const char* valTypeToStr(ValType type);

#endif
