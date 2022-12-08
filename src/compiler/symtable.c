
#include "common.h"
#include "errors.h"
#include "memory.h"

typedef struct _symtab_elem {
    const char* key;
    // VarIdx idx;
    int type;
    void* value;
    struct _symtab_elem* left;
    struct _symtab_elem* right;
} SymTabNode;

typedef struct _context {
    const char* name;
    struct _context* next;
    struct _context* prev;
} Context;

typedef struct _compound_name {
    char* name;
    char** list;
    size_t cap;
    size_t len;
} CompoundName;

static SymTabNode* symtab = NULL;
static Context* first = NULL;
static Context* last = NULL;

static char name_buffer[1024];
static void build_name(Context* cont, const char* name) {
    if(cont != NULL) {
        strcat(name_buffer, cont->name);
        strcat(name_buffer, ".");
        build_name(cont->next, name);
    }
    else {
        strcat(name_buffer, name);
    }
}

static const char* create_name(const char* name) {
    memset(name_buffer, 0, sizeof(name_buffer));
    name_buffer[0] = '.';
    if(first != NULL)
        build_name(first, name);
    else
        strcat(name_buffer, name);

    return name_buffer;
}

// left is always the value that is being searched for
static int comp_names(const char* left, const char* right) {
    int llen = strlen(left);
    int rlen = strlen(right);

    while(left[llen] == right[rlen]) {
        rlen--;
        llen--;
        if(llen == 0 || rlen == 0)
            return 0; // match
    }

    return left[llen] - right[rlen];
}

static void dump_table(SymTabNode* node) {
    if(node->left != NULL)
        dump_table(node->left);
    if(node->right != NULL)
        dump_table(node->right);

    printf("    %s <%d> <%p>\n", node->key, node->type, node->value);
    // printVal(getVar(&vm->vstore, node->idx));
    // printf("\n");
}

static void add_node(SymTabNode* tree, SymTabNode* node) {
    int x = strcmp(tree->key, node->key);
    if(x > 0) {
        if(tree->right != NULL)
            add_node(tree->right, node);
        else {
            tree->right = node;
            return;
        }
    }
    else if(x < 0) {
        if(tree->left != NULL)
            add_node(tree->left, node);
        else {
            tree->left = node;
            return;
        }
    }
    else {
        error("symbol \"%s\" is already defined", node->key);
        return;
    }
}

static SymTabNode* find_node(SymTabNode* node, const char* key) {
    int x = comp_names(node->key, key);
    if(x > 0) {
        if(node->right != NULL)
            return find_node(node->right, key);
        else
            return NULL;
    }
    else if(x < 0) {
        if(node->left != NULL)
            return find_node(node->left, key);
        else
            return NULL;
    }
    else
        return node;
}

/*************************************************************
 * Interface
 */


// symbol definition
void addSym(const char* key, int idx, void* value) {
    SymTabNode* node = _alloc_ds(SymTabNode);
    node->key = _copy_str(create_name(key));
    node->type = idx;
    node->value = value;

    if(symtab != NULL)
        add_node(symtab, node);
    else
        symtab = node;
}

// symbol reference
int symToType(const char* key) {
    SymTabNode* node = find_node(symtab, create_name(key));

    if(node != NULL)
        return node->type;
    else
        return 0; // error Var
}

// symbol reference
void* symToVal(const char* key) {
    SymTabNode* node = find_node(symtab, key);
    void* val;

    if(node != NULL)
        val = node->value;
    else
        val = NULL;

    return val;
}

void dumpSymtab() {
    printf("Dump Symbol Table\n");
    dump_table(symtab);
    printf("----------- end dump -----------\n");
}

void pushContext(const char* name) {
    Context* cont = _alloc_ds(Context);
    cont->name = _copy_str(name);

    if(first != NULL) {
        last->next = cont;
        cont->prev = last;
        last = cont;
    }
    else {
        last = first = cont;
    }
}

void popContext() {
    Context* cont = last;

    if(cont != NULL) {
        last = last->prev;
        if(last != NULL) {
            last->next = NULL;
        }
    }
}

CompoundName* createCompoundName(const char* str) {

    CompoundName* cn = _alloc_ds(CompoundName);

    cn->name = _copy_str(str);
    cn->cap = 0x04;
    cn->len = 1;
    cn->list = _alloc_ds_array(char*, cn->cap);
    cn->list[0] = cn->name;

    return cn;
}
