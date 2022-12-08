
#include "gas.h"
#include <assert.h>

static void add_node(Module* mod, Object* obj) {
    if(mod->first != NULL) {
        mod->last->next = obj;
        mod->last = obj;
    }
    else {
        mod->first = obj;
        mod->last = obj;
    }
}

const char* objTypeToStr(ObjectType type) {
    return (type == OT_LABEL)    ? "LABEL" :
    (type == OT_PP_MARKER)       ? "PP_MARKER" :
    (type == OT_CLASS0_INSTR)    ? "CLASS0_INSTR" :
    (type == OT_CLASS1_INSTR)    ? "CLASS1_INSTR" :
    (type == OT_CLASS2_INSTR)    ? "CLASS2_INSTR" :
    (type == OT_CLASS3_INSTR)    ? "CLASS3_INSTR" :
    (type == OT_CLASS4_INSTR)    ? "CLASS4_INSTR" :
    (type == OT_CLASS5_INSTR)    ? "CLASS5_INSTR" :
    (type == OT_CLASS6_INSTR)    ? "CLASS6_INSTR" :
    (type == OT_CLASS7_INSTR)    ? "CLASS7_INSTR" :
    (type == OT_CLASS8_INSTR)    ? "CLASS8_INSTR" :
    (type == OT_CLASS9_INSTR)    ? "CLASS9_INSTR" :
    (type == OT_DATA_DEFINITION) ? "DATA_DEFINITION" :
                                   "UNKNOWN";
}

void printModule(Module* mod) {
    printf("\n------- Dump AST -------\n");
    if(mod->first != NULL) {
        Object* obj = mod->first;
        while(obj != NULL) {
            printf("type: %s\t", objTypeToStr(obj->type));
            //             if(obj->type == OT_LABEL) {
            //                 Label* lab = (Label*)obj;
            //                 printf("name: %s val: %d", lab->name, lab->val);
            //             }
            switch(obj->type) {
                case OT_LABEL: {
                    Label* ptr = (Label*)obj;
                    printf("\tname: %s,\taddress: %d", ptr->name, ptr->addr);
                } break;
                case OT_PP_MARKER: {
                    PPMarker* ptr = (PPMarker*)obj;
                    printf("line: %d,\tfile: %s", ptr->line, ptr->file);
                } break;
                case OT_CLASS0_INSTR: {
                    Class0* ptr = (Class0*)obj;
                    printf("addr: %d op: %s", ptr->addr, opToStr(ptr->op));
                } break;
                case OT_CLASS1_INSTR: {
                    Class1* ptr = (Class1*)obj;
                    printf("addr: %d op: %s reg:%s", ptr->addr,
                           opToStr(ptr->op), regToStr(ptr->reg));
                } break;
                case OT_CLASS2_INSTR: {
                    Class2* ptr = (Class2*)obj;
                    printf("addr: %d op: %s left:%s, right:%s", ptr->addr,
                           opToStr(ptr->op), regToStr(ptr->left), regToStr(ptr->right));
                } break;
                case OT_CLASS3_INSTR: {
                    Class3* ptr = (Class3*)obj;
                    printf("addr: %d op: %s dest:%s, left:%s, right:%s",
                           ptr->addr, opToStr(ptr->op), regToStr(ptr->dest),
                           regToStr(ptr->left), regToStr(ptr->right));
                } break;
                case OT_CLASS4_INSTR: {
                    Class4* ptr = (Class4*)obj;
                    printf("addr: %d op: %s num:%d", ptr->addr,
                           opToStr(ptr->op), ptr->tnum);
                } break;

                case OT_CLASS5_INSTR: {
                    Class5* ptr = (Class5*)obj;
                    printf("addr: %d op: %s reg:%s, sym:%s idx: %d ", ptr->addr,
                           opToStr(ptr->op), regToStr(ptr->reg), ptr->sym, ptr->idx);
                    printVal(ptr->val);
                } break;
                case OT_CLASS6_INSTR: {
                    Class6* ptr = (Class6*)obj;
                    printf("addr: %d op: %s reg:%s, ", ptr->addr,
                           opToStr(ptr->op), regToStr(ptr->reg));
                    printVal(ptr->val);
                } break;
                case OT_CLASS7_INSTR: {
                    Class7* ptr = (Class7*)obj;
                    printf("addr: %d op: %s sym:%s idx: %d ", ptr->addr,
                           opToStr(ptr->op), ptr->sym, ptr->idx);
                    printVal(ptr->val);
                    printf(", reg:%s", regToStr(ptr->reg));
                } break;

                case OT_CLASS8_INSTR: {
                    Class8* ptr = (Class8*)obj;
                    printf("addr: %d op: %s symbol:%s addr: %d", ptr->addr,
                           opToStr(ptr->op), ptr->sym, ptr->addr);
                } break;

                case OT_CLASS9_INSTR: {
                    Class9* ptr = (Class9*)obj;
                    printf("addr: %d op: %s dest:%s base: %s, ofst: %d",
                           ptr->addr, opToStr(ptr->op), regToStr(ptr->dest),
                           regToStr(ptr->base), ptr->val);
                } break;
                    //                 case OT_CLASS5_INSTR: {
                    //                         Class5* ptr = (Class5*)obj;
                    //                         printf("addr: %d op: %s ", ptr->addr, opToStr(ptr->op));
                    //                         printVal(ptr->val);
                    //                     }
                    //                     break;
                case OT_DATA_DEFINITION: {
                    DataDef* ptr = (DataDef*)obj;
                    printf("symbol:%s value:", ptr->name);
                    printVal(ptr->val);
                    printf("\trefs: %d index: %d", ptr->references, ptr->idx);
                } break;
                default:
                    printf("unknown object type!: %d\n", obj->type);
                    return;
            }

            obj = obj->next;
            printf("\n");
        }
    }
    printf("-------- End dump --------\n");
}

Module* createModule() {
    Module* mod = _alloc_ds(Module);
    mod->obj.type = 0;
    mod->obj.next = NULL;

    mod->first = NULL;
    mod->last = NULL;

    return mod;
}


void addLabel(Module* mod, const char* name) {
    Label* lab = _alloc_ds(Label);
    lab->obj.type = OT_LABEL;
    lab->obj.next = NULL;

    lab->name = _copy_str(name);
    lab->addr = 0;

    add_node(mod, (Object*)lab);
}

void addClass0(Module* mod, OpCode op) {
    Class0* obj = _alloc_ds(Class0);
    obj->obj.type = OT_CLASS0_INSTR;
    obj->obj.next = NULL;

    obj->op = op;

    add_node(mod, (Object*)obj);
}

void addClass1(Module* mod, OpCode op, Reg reg) {
    Class1* obj = _alloc_ds(Class1);
    obj->obj.type = OT_CLASS1_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->reg = reg;

    add_node(mod, (Object*)obj);
}

void addClass2(Module* mod, OpCode op, Reg left, Reg right) {
    Class2* obj = _alloc_ds(Class2);
    obj->obj.type = OT_CLASS2_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->left = left;
    obj->right = right;

    add_node(mod, (Object*)obj);
}

void addClass3(Module* mod, OpCode op, Reg dest, Reg left, Reg right) {
    Class3* obj = _alloc_ds(Class3);
    obj->obj.type = OT_CLASS3_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->dest = dest;
    obj->left = left;
    obj->right = right;

    add_node(mod, (Object*)obj);
}

void addClass4(Module* mod, OpCode op, int tnum) {
    Class4* obj = _alloc_ds(Class4);
    obj->obj.type = OT_CLASS4_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->tnum = tnum;

    add_node(mod, (Object*)obj);
}

void addClass5(Module* mod, OpCode op, Reg reg, const char* symb) {
    Class5* obj = _alloc_ds(Class5);
    obj->obj.type = OT_CLASS5_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->reg = reg;
    obj->sym = _copy_str(symb);

    add_node(mod, (Object*)obj);
}

void addClass6(Module* mod, OpCode op, Reg reg, Value* val) {
    Class6* obj = _alloc_ds(Class6);
    obj->obj.type = OT_CLASS6_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->reg = reg;
    obj->val = val;

    add_node(mod, (Object*)obj);
}

void addClass7(Module* mod, OpCode op, const char* symb, Reg reg) {
    Class7* obj = _alloc_ds(Class7);
    obj->obj.type = OT_CLASS7_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->sym = _copy_str(symb);
    obj->reg = reg;

    add_node(mod, (Object*)obj);
}

void addClass8(Module* mod, OpCode op, const char* symb) {
    Class8* obj = _alloc_ds(Class8);
    obj->obj.type = OT_CLASS8_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->sym = _copy_str(symb);

    add_node(mod, (Object*)obj);
}

void addClass9(Module* mod, OpCode op, Reg dest, Reg base, uint16_t val) {
    Class9* obj = _alloc_ds(Class9);
    obj->obj.type = OT_CLASS9_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->dest = dest;
    obj->base = base;
    obj->val = val;

    add_node(mod, (Object*)obj);
}

void addClass10(Module* mod, OpCode op, const char* symb) {
    Class10* obj = _alloc_ds(Class10);
    obj->obj.type = OT_CLASS10_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->sym = symb;

    add_node(mod, (Object*)obj);
}

void addClass11(Module* mod, OpCode op, Value* val) {
    Class11* obj = _alloc_ds(Class11);
    obj->obj.type = OT_CLASS11_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->val = val;

    add_node(mod, (Object*)obj);
}

void addClass12(Module* mod, OpCode op, const char* symb, Value* val) {
    Class12* obj = _alloc_ds(Class12);
    obj->obj.type = OT_CLASS12_INSTR;
    obj->obj.next = NULL;

    obj->op = op;
    obj->sym = symb;
    obj->ival = val;

    add_node(mod, (Object*)obj);
}

void addPPMarker(Module* mod, int line, const char* fname) {
    PPMarker* obj = _alloc_ds(PPMarker);
    obj->obj.type = OT_PP_MARKER;
    obj->obj.next = NULL;

    obj->line = line;
    obj->file = _copy_str(fname);

    add_node(mod, (Object*)obj);
}

DataDef* addDataDef(Module* mod, Value* val, const char* name) {
    DataDef* obj = _alloc_ds(DataDef);
    obj->obj.type = OT_DATA_DEFINITION;
    obj->obj.next = NULL;

    obj->name = _copy_str(name);
    obj->val = val;
    obj->references = 0;

    obj->idx = addValTab(val);
    addSymVal(name, val, obj->idx);

    add_node(mod, (Object*)obj);
    return obj;
}

void assignDataDecl(DataDef* data, Value* val) {
    memcpy(data->val, val, sizeof(Value));
}
