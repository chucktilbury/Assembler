/*
 * This file implements the symbol table.
 */
#include "symbol_tab.h"
#include "memory.h"

/*
 * Hash* HashCreate();
 * void HashDestroy(Hash* table);
 * HashResult HashInsert(Hash* table, const char* key, void* data, size_t size);
 * HashResult HashFind(Hash* tab, const char* key, void* data, size_t size);
 * HashResult HashRemove(Hash* tab, const char* key);
 */

SymTab* symtab = NULL;

void initSymbolTab() {

    symtab = (SymTab*)HashCreate();
}

int addSymbol(const char* key, int type, void* data) {

    Symbol* sym = _alloc_ds(Symbol);
    sym->key = _copy_str(key);
    sym->type = type;
    sym->value = data;

    return (int)HashInsert(symtab, key, sym, sizeof(Symbol));
}

Symbol* getSymbol(const char* key) {

    Symbol* sym = _alloc_ds(Symbol);
    HashResult r = HashFind(symtab, key, sym, sizeof(Symbol));

    if(r == HASH_OK)
        return sym;
    else
        return NULL;
}

int getSymbolType(const char* key) {

    Symbol* sym = _alloc_ds(Symbol);
    HashResult r = HashFind(symtab, key, sym, sizeof(Symbol));

    if(r == HASH_OK)
        return sym->type;
    else if(r == HASH_NF)
        return -1; // type cannot be negative
    else
        return -2;
}

void* getSymbolData(const char* key) {

    Symbol* sym = _alloc_ds(Symbol);
    HashResult r = HashFind(symtab, key, sym, sizeof(Symbol));

    if(r == HASH_OK)
        return sym->value;
    else
        return NULL;
}

void dumpSymTab() {

    HashDump((Hash*)symtab);
}

