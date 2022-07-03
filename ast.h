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
    OT_CLASS7A_INSTR,
    OT_CLASS7B_INSTR,
    OT_CLASS7C_INSTR,
    OT_DATA_DEFINITION,
} ObjectType;

typedef enum {
    REG_0,
    REG_1,
    REG_2,
    REG_3,
    REG_4,
    REG_5,
    REG_6,
    REG_7,
    REG_8,
    REG_9,
    REG_10,
    REG_11,
    REG_12,
    REG_13,
    REG_14,
    REG_15,
} Register;


typedef uint8_t Reg;

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
} DataDef;

typedef struct {
    Object obj;
    OpCode op;
} Class0;

typedef struct {
    Object obj;
    OpCode op;
    Reg dest;
    Reg left;
    Reg right;
} Class1;

typedef struct {
    Object obj;
    OpCode op;
    Reg left;
    Reg right;
} Class2;

typedef struct {
    Object obj;
    OpCode op;
    Reg reg;
} Class3;

typedef struct {
    Object obj;
    OpCode op;
    const char* sym;
    uint32_t addr;
} Class4;

typedef struct {
    Object obj;
    OpCode op;
    Value* val;
    ValIdx idx;
} Class5;

typedef struct {
    Object obj;
    OpCode op;
    int tnum;
} Class6;

typedef struct {
    Object obj;
    OpCode op;
    Reg reg;
    const char* sym;
    Value* val;
    ValIdx idx;
} Class7a;

typedef struct {
    Object obj;
    OpCode op;
    Reg reg;
    Value* val;
} Class7b;

typedef struct {
    Object obj;
    OpCode op;
    const char* sym;
    Value* val;
    Reg reg;
    ValIdx idx;
} Class7c;

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
void addClass1(Module* mod, OpCode op, Reg dest, Reg left, Reg right);
void addClass2(Module* mod, OpCode op, Reg left, Reg right);
void addClass3(Module* mod, OpCode op, Reg reg);
void addClass4(Module* mod, OpCode op, const char* symb);
void addClass5(Module* mod, OpCode op, Value* val);
void addClass6(Module* mod, OpCode op, int tnum);
void addClass7a(Module* mod, OpCode op, Reg reg, const char* symb);
void addClass7b(Module* mod, OpCode op, Reg reg, Value* val);
void addClass7c(Module* mod, OpCode op, const char* symb, Reg reg);
void addPPMarker(Module* mod, int line, const char* fname);
DataDef* addDataDef(Module* mod, Value* val, const char* name);
void assignDataDecl(DataDef* data, Value* val);
const char* regToStr(uint8_t reg);

#endif
