#ifndef AST_H
#define AST_H

typedef enum {
    OT_LABEL,
    OT_PP_MARKER,
    OT_CLASS0_INSTR,
    OT_CLASS1_INSTR,
    OT_CLASS2_INSTR,
    OT_CLASS3_INSTR,
    OT_CLASS4_INSTR,
    OT_CLASS5_INSTR,
    OT_CLASS6_INSTR,
    OT_CLASS7_INSTR,
    OT_CLASS8_INSTR,
    OT_DATA_DEFINITION,
} ObjectType;

typedef struct _object {
    ObjectType type;
    struct _object* next;
} Object;

typedef struct {
    Object obj;
    Object* first;
    Object* last;
} Module;

typedef struct {
    Object obj;
    const char* name;
    uint32_t addr;
} Label;

typedef struct {
    Object obj;
    const char* name;
    Value* val;
    ValIdx idx;
    uint32_t references;
    const char* str;
    StrIdx sidx;
} DataDef;

typedef struct {
    Object obj;
    OpCode op;
    uint32_t addr;
} Class0;

typedef struct {
    Object obj;
    OpCode op;
    Reg reg;
    uint32_t addr;
} Class1;

typedef struct {
    Object obj;
    OpCode op;
    Reg left;
    Reg right;
    uint32_t addr;
} Class2;

typedef struct {
    Object obj;
    OpCode op;
    Reg dest;
    Reg left;
    Reg right;
    uint32_t addr;
} Class3;

typedef struct {
    Object obj;
    OpCode op;
    int tnum;
    uint32_t addr;
} Class4;

typedef struct {
    Object obj;
    OpCode op;
    Reg reg;
    const char* sym;
    Value* val;
    ValIdx idx;
    uint32_t addr;
} Class5;

typedef struct {
    Object obj;
    OpCode op;
    Reg reg;
    Value* val;
    uint32_t addr;
} Class6;

typedef struct {
    Object obj;
    OpCode op;
    const char* sym;
    Value* val;
    Reg reg;
    ValIdx idx;
    uint32_t addr;
} Class7;

typedef struct {
    Object obj;
    OpCode op;
    const char* sym;
    uint32_t addr;
} Class8;

typedef struct {
    Object obj;
    int line;
    const char* file;
} PPMarker;

Module* createModule();
void printModule(Module* mod);
const char* objTypeToStr(ObjectType type);
void addLabel(Module* mod, const char* name);
void addClass0(Module* mod, OpCode op);
void addClass1(Module* mod, OpCode op, Reg reg);
void addClass2(Module* mod, OpCode op, Reg left, Reg right);
void addClass3(Module* mod, OpCode op, Reg dest, Reg left, Reg right);
void addClass4(Module* mod, OpCode op, int tnum);
void addClass5(Module* mod, OpCode op, Reg reg, const char* symb);
void addClass6(Module* mod, OpCode op, Reg reg, Value* val);
void addClass7(Module* mod, OpCode op, const char* symb, Reg reg);
void addClass8(Module* mod, OpCode op, const char* symb);
void addPPMarker(Module* mod, int line, const char* fname);
DataDef* addDataDef(Module* mod, Value* val, const char* name);
void assignDataDecl(DataDef* data, Value* val);
const char* regToStr(uint8_t reg);

#endif
