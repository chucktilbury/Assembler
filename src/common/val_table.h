#ifndef VAL_BUFFER_H
#define VAL_BUFFER_H

typedef enum {
    ERROR,
    NUM,
    BOOL,
    STRING,
    ADDR,
    UDATA, // this is only used by traps
} ValType;

typedef Index ValIdx;

typedef struct {
    ValType type;
    bool isAssigned;
    bool isConst;
    int references;
    union {
        double num;
        bool bval;
        // StrIdx str;
        const char* str;
        int stridx;
        unsigned int addr;
        void* usr;
    } data;
} Value;

void initValTab();
void saveValTab(FILE* fp);
void loadValTab(FILE* fp);
ValIdx addValTab(Value* val);
Value* getValTab(ValIdx idx);
void dumpValTab();
void printVal(Value* val);
Value* createValue(ValType type);
Value* castVal(ValType type, Value* val);
const char* valTypeToStr(ValType type);
void assignVal(ValIdx idx, Value* val);
const char* valToStr(Value* val);

#endif
