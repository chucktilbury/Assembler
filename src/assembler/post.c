
#include "gas.h"

#define TREE_ERROR (((uint32_t)0x00)-1)

extern Module* module;

static int size_table[] = {
    [OT_LABEL] = 0,
    [OT_PP_MARKER] = 0,
    [OT_CLASS0_INSTR] = sizeof(uint8_t),
    [OT_CLASS1_INSTR] = sizeof(uint8_t)+sizeof(uint8_t),
    [OT_CLASS2_INSTR] = sizeof(uint8_t)+sizeof(uint8_t),
    [OT_CLASS3_INSTR] = sizeof(uint8_t)+sizeof(uint16_t),
    [OT_CLASS4_INSTR] = sizeof(uint8_t)+sizeof(uint16_t),
    [OT_CLASS5_INSTR] = sizeof(uint8_t)+sizeof(uint8_t)+sizeof(ValIdx),
    [OT_CLASS6_INSTR] = sizeof(uint8_t)+sizeof(uint8_t)+sizeof(Value),
    [OT_CLASS7_INSTR] = sizeof(uint8_t)+sizeof(uint8_t)+sizeof(ValIdx),
    [OT_CLASS8_INSTR] = sizeof(uint8_t)+sizeof(uint32_t),
    [OT_CLASS9_INSTR] = sizeof(uint8_t)+sizeof(uint8_t)+sizeof(uint16_t),
    [OT_CLASS10_INSTR] = sizeof(uint8_t)+sizeof(ValIdx),
    [OT_CLASS11_INSTR] = sizeof(uint8_t)+sizeof(Value),
    [OT_CLASS12_INSTR] = sizeof(uint8_t)+sizeof(ValIdx)+sizeof(Value),
    [OT_DATA_DEFINITION] = 0
};

typedef struct _lab_tab {
    const char* key;
    uint32_t val;
    struct _lab_tab* left;
    struct _lab_tab* right;
} LabTab;

static LabTab* ltable;

static void add_lnode(LabTab* root, LabTab* node)
{
    int val = strcmp(root->key, node->key);
    if(val > 0) {
        if(root->right != NULL)
            add_lnode(root->right, node);
        else
            root->right = node;
    }
    else if(val < 0) {
        if(root->left != NULL)
            add_lnode(root->left, node);
        else
            root->left = node;
    }
    else
        syntaxError("symbol %s has already been defined", node->key);
}

static uint32_t find_lnode(LabTab* tree, const char* key)
{
    int val = strcmp(tree->key, key);
    if(val > 0) {
        if(tree->right != NULL)
            return find_lnode(tree->right, key);
        else
            return TREE_ERROR;
    }
    else if(val < 0) {
        if(tree->left != NULL)
            return find_lnode(tree->left, key);
        else
            return TREE_ERROR;
    }
    else
        return tree->val;
}

static void addLabTab(const char* key, uint32_t val)
{
    LabTab* node = _alloc_ds(LabTab);
    node->key = _copy_str(key);
    node->val = val;

    if(ltable != NULL)
        add_lnode(ltable, node);
    else
        ltable = node;
}

static uint32_t findLabTab(const char* key)
{
    if(ltable != NULL)
        return find_lnode(ltable, key);
    else
        return TREE_ERROR;
}


typedef struct _val_tab {
    const char* key;
    Value* val;
    ValIdx idx;
    struct _val_tab* left;
    struct _val_tab* right;
} ValTab;

static ValTab* vtable;

static void add_vnode(ValTab* root, ValTab* node)
{
    int val = strcmp(root->key, node->key);
    //printf(">>>> val: %d key: %s\n", val, node->key);
    if(val > 0) {
        if(root->right != NULL)
            add_vnode(root->right, node);
        else {
            //printf(">>>> (right) node->key: %s\n", node->key);
            root->right = node;
        }
    }
    else if(val < 0) {
        if(root->left != NULL)
            add_vnode(root->left, node);
        else {
            //printf(">>>> (left) node->key: %s\n", node->key);
            root->left = node;
        }
    }
    else
        syntaxError("symbol %s has already been defined", node->key);
}

static ValTab* find_vnode(ValTab* tree, const char* key)
{
    int val = strcmp(tree->key, key);
    if(val > 0) {
        if(tree->right != NULL)
            return find_vnode(tree->right, key);
        else
            return NULL;
    }
    else if(val < 0) {
        if(tree->left != NULL)
            return find_vnode(tree->left, key);
        else
            return NULL;
    }
    else
        return tree;
}

void addSymVal(const char* key, Value* val, ValIdx idx)
{
    ValTab* node = _alloc_ds(ValTab);
    node->key = _copy_str(key);
    node->val = val;
    node->idx = idx;

    if(vtable != NULL)
        add_vnode(vtable, node);
    else
        vtable = node;
}

ValTab* findValTab(const char* key)
{
    if(vtable != NULL)
        return find_vnode(vtable, key);
    else
        return NULL;
}

ValIdx findValTabIdx(const char* key)
{
    //printf("find: %s\n", key);
    if(vtable != NULL) {
        ValTab* v = find_vnode(vtable, key);
        return v->idx;
    }
    else
        return ((ValIdx)0) - 1;
}

static void dump_vtable(ValTab* node)
{
    if(node->left != NULL)
        dump_vtable(node->left);
    if(node->right != NULL)
        dump_vtable(node->right);

    printf("    index: %d\t\"", node->idx);
    printVal(node->val);
    printf("\"\tkey: %s\n", node->key);
}

static void dump_ltable(LabTab* node)
{
    if(node->left != NULL)
        dump_ltable(node->left);
    if(node->right != NULL)
        dump_ltable(node->right);

    printf("    address: %-3d\tkey: %s\n", node->val, node->key);
}

void dumpPostTables()
{
    printf("\n------- Dump Post Tables -------\n");
    printf("------- Values\n");
    if(vtable != NULL)
        dump_vtable(vtable);
    else
        printf("    value table is empty\n");
    printf("------- Labels\n");
    if(ltable != NULL)
        dump_ltable(ltable);
    else
        printf("    label table is empty\n");
    printf("------- End Dump -------\n");
}

/*
 * Write the instructions to the instruction stream and get the address of all
 * of the labels.
 */
static void instrs(Module* mod)
{
    Object* obj = mod->first;
    while(obj != NULL) {

        switch(obj->type) {
            case OT_PP_MARKER:
            case OT_DATA_DEFINITION:
            case OT_LABEL:
                break;
            case OT_CLASS0_INSTR: {
                    Class0* ptr = (Class0*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                }
                break;
            case OT_CLASS1_INSTR: {
                    Class1* ptr = (Class1*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInst8(ptr->reg & 0xF);
                }
                break;
            case OT_CLASS2_INSTR: {
                    Class2* ptr = (Class2*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInst8((ptr->right & 0xF) |
                                ((ptr->left & 0xF) << 4));
                }
                break;
            case OT_CLASS3_INSTR: {
                    Class3* ptr = (Class3*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInst16((ptr->right & 0xF) |
                                ((ptr->left & 0xF) << 4) |
                                ((ptr->dest & 0xF) << 8));
                }
                break;
            case OT_CLASS4_INSTR: {
                    Class4* ptr = (Class4*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInst16(ptr->tnum);
                }
                break;
            case OT_CLASS5_INSTR: {
                    Class5* ptr = (Class5*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInst8(ptr->reg & 0x0F);
                    writeInstObj(&ptr->idx, sizeof(ValIdx));
                }
                break;
            case OT_CLASS6_INSTR: {
                    Class6* ptr = (Class6*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInst8(ptr->reg & 0x0F);
                    //Value* val = createValue(ERROR);
                    writeInstObj(ptr->val, sizeof(Value));
                }
                break;
            case OT_CLASS7_INSTR: {
                    Class7* ptr = (Class7*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInstObj(&ptr->idx, sizeof(ValIdx));
                    writeInst8(ptr->reg & 0x0F);
                }
                break;
            case OT_CLASS8_INSTR: {
                    Class8* ptr = (Class8*)obj;
                    //ptr->iaddr = getAddr();
                    writeInst8(ptr->op);
                    writeInst32(ptr->addr);
                }
                break;
            case OT_CLASS9_INSTR: {
                    Class9* ptr = (Class9*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInst8((ptr->base & 0xF) |
                                ((ptr->dest & 0xF) << 4));
                    writeInst16(ptr->val);
                }
                break;
            case OT_CLASS10_INSTR: {
                    Class10* ptr = (Class10*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInstObj(&ptr->idx, sizeof(ValIdx));
                }
                break;
            case OT_CLASS11_INSTR: {
                    Class11* ptr = (Class11*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInstObj(ptr->val, sizeof(Value));
                }
                break;
            case OT_CLASS12_INSTR: {
                    Class12* ptr = (Class12*)obj;
                    ptr->addr = getAddr();
                    writeInst8(ptr->op);
                    writeInstObj(ptr->ival, sizeof(Value));
                    writeInstObj(&ptr->idx, sizeof(ValIdx));
                }
                break;
            default:
                printf("unknown instr object type!: %d\n", obj->type);
                return;
        }

        obj = obj->next;
    }
}

/*
 * Scan the labels and assign the address.
 */
static void label_scan(Module* mod)
{
    Object* obj = mod->first;
    while(obj != NULL) {

        switch(obj->type) {
            case OT_PP_MARKER:
            case OT_DATA_DEFINITION:
            case OT_LABEL:
            case OT_CLASS0_INSTR:
            case OT_CLASS1_INSTR:
            case OT_CLASS2_INSTR:
            case OT_CLASS3_INSTR:
            case OT_CLASS4_INSTR:
            case OT_CLASS6_INSTR:
            case OT_CLASS9_INSTR:
            case OT_CLASS11_INSTR:
                break;
            case OT_CLASS5_INSTR: {
                    Class5* ptr = (Class5*)obj;
                    ValTab* tab = findValTab(ptr->sym);
                    if(tab != NULL) {
                        ptr->val = tab->val;
                        ptr->idx = tab->idx;
                    }
                    else
                        syntaxError("value for %s is not defined", ptr->sym);
                }
                break;
            case OT_CLASS7_INSTR: {
                    Class7* ptr = (Class7*)obj;
                    ValTab* tab = findValTab(ptr->sym);
                    if(tab != NULL) {
                        ptr->val = tab->val;
                        ptr->idx = tab->idx;
                    }
                    else
                        syntaxError("value for %s is not defined", ptr->sym);
                }
                break;
            case OT_CLASS8_INSTR: {
                    Class8* ptr = (Class8*)obj;
                    uint32_t addr = findLabTab(ptr->sym);
                    if(addr != TREE_ERROR)
                        ptr->addr = addr;
                    else
                        syntaxError("label %s definition not found", ptr->sym);
                }
                break;
            case OT_CLASS10_INSTR: {
                    Class10* ptr = (Class10*)obj;
                    ValTab* tab = findValTab(ptr->sym);
                    if(tab != NULL) {
                        ptr->idx = tab->idx;
                        ptr->val = tab->val;
                    }
                    else
                        syntaxError("value %s definition not found", ptr->sym);
                }
                break;
            case OT_CLASS12_INSTR: {
                    Class12* ptr = (Class12*)obj;
                    // Map the symbol to an index in the table.
                    ValTab* tab = findValTab(ptr->sym);
                    if(tab != NULL) {
                        ptr->idx = tab->idx;
                        ptr->oval = tab->val;
                    }
                    else
                        syntaxError("value %s definition not found", ptr->sym);

                }
                break;
            default:
                printf("unknown label object type!: %d\n", obj->type);
                return;
        }

        obj = obj->next;
    }
}

static void format_strs(Module* mod)
{
    Object* obj = mod->first;

    while(obj != NULL) {

        switch(obj->type) {
            case OT_DATA_DEFINITION: {
                    DataDef* ptr = (DataDef*)obj;
                    if(ptr->val->type == STRING && ptr->val->isAssigned)
                        ptr->val->data.str = preformat_str(ptr->val->data.str);
                }
                break;
            case OT_PP_MARKER:
            case OT_LABEL:
            case OT_CLASS0_INSTR:
            case OT_CLASS1_INSTR:
            case OT_CLASS2_INSTR:
            case OT_CLASS3_INSTR:
            case OT_CLASS4_INSTR:
            case OT_CLASS5_INSTR:
            case OT_CLASS6_INSTR:
            case OT_CLASS7_INSTR:
            case OT_CLASS8_INSTR:
            case OT_CLASS9_INSTR:
            case OT_CLASS10_INSTR:
            case OT_CLASS11_INSTR:
            case OT_CLASS12_INSTR:
                break;
            default:
                printf("unknown format object type!: %d\n", obj->type);
                return;
        }

        obj = obj->next;
    }
}

/*
 * Scan the references to labels and set their addresses.
 */
static void addr_scan(Module* mod)
{
    uint32_t addr = 0;
    Object* obj = mod->first;
    while(obj != NULL) {

        switch(obj->type) {
                case OT_PP_MARKER:
                case OT_DATA_DEFINITION:
                    break;
                case OT_LABEL: {
                        Label* ptr = (Label*)obj;
                        ptr->addr = addr;
                        //printf("address: %d\n", addr);
                        addLabTab(ptr->name, ptr->addr);
                    }
                    break;
                case OT_CLASS0_INSTR: addr += size_table[OT_CLASS0_INSTR]; break;
                case OT_CLASS1_INSTR: addr += size_table[OT_CLASS1_INSTR]; break;
                case OT_CLASS2_INSTR: addr += size_table[OT_CLASS2_INSTR]; break;
                case OT_CLASS3_INSTR: addr += size_table[OT_CLASS3_INSTR]; break;
                case OT_CLASS4_INSTR: addr += size_table[OT_CLASS4_INSTR]; break;
                case OT_CLASS5_INSTR: addr += size_table[OT_CLASS5_INSTR]; break;
                case OT_CLASS6_INSTR: addr += size_table[OT_CLASS6_INSTR]; break;
                case OT_CLASS7_INSTR: addr += size_table[OT_CLASS7_INSTR]; break;
                case OT_CLASS8_INSTR: addr += size_table[OT_CLASS8_INSTR]; break;
                case OT_CLASS9_INSTR: addr += size_table[OT_CLASS9_INSTR]; break;
                case OT_CLASS10_INSTR: addr += size_table[OT_CLASS10_INSTR]; break;
                case OT_CLASS11_INSTR: addr += size_table[OT_CLASS11_INSTR]; break;
                case OT_CLASS12_INSTR: addr += size_table[OT_CLASS12_INSTR]; break;
            default:
                printf("unknown addr object type!: %d\n", obj->type);
                return;
        }

        obj = obj->next;
    }
}

#include "cmdline.h"

void doPostProcess(Module* mod)
{
    if(mod->first == NULL)
        fatalError("module is corrupt or empty");

    addr_scan(mod);
    label_scan(mod);
    instrs(mod);
    format_strs(mod);

    if(getNumParam("verbose")) {
        printModule(mod);
        dumpValTab();
        dumpPostTables();
        dumpStrTab();
    }
}
