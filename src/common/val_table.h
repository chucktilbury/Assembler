#ifndef VAL_BUFFER_H
#define VAL_BUFFER_H

typedef enum {
    ERROR,
    INT,
    UINT,
    FLOAT,
    BOOL,
    STRING,
    UDATA, // this is only used by traps
} ValType;

typedef Index ValIdx;

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
        void* usr;
    } data;
} Value;

void initValBuf();
void saveValBuf(FILE* fp);
void loadValBuf(FILE* fp);
ValIdx addValBuf(Value* val);
Value* getValBuf(ValIdx idx);
void dumpValBuf();
void printVal(Value* val);
Value* createValue(ValType type);
Value* castVal(ValType type, Value* val);
const char* valTypeToStr(ValType type);
void assignVal(ValIdx idx, Value* val);
const char* valToStr(Value* val);

#endif
