#ifndef _SYMBOL_TAB_H_
#define _SYMBOL_TAB_H_

#include "hash.h"

typedef struct {
    const char* key;
    void* value;
} Symbol;

typedef Hash SymTab;

// defined in symbol_tab.c
extern SymTab* symtab;

/*
 * Hash* HashCreate();
 * void HashDestroy(Hash* table);
 * HashResult HashInsert(Hash* table, const char* key, void* data, size_t size);
 * HashResult HashFind(Hash* tab, const char* key, void* data, size_t size);
 * HashResult HashRemove(Hash* tab, const char* key);
 */
#define initSymTab()                    \
    do {                                \
        symtab = (SymTab*)HashCreate(); \
    } while(0)
#define addSymTab(k, d) HashInsert(symtab, (k), (d), sizeof(Symbol))
#define getSymTab(k, d) HashFind(symtab, (k), (d), sizeof(Symbol))
// symbols are never removed and gc takes care of cleanup

void dumpSymTab(SymTab* st);

#endif