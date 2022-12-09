#ifndef _COMPILER_SYMBOL_TAB_H
#define _COMPILER_SYMBOL_TAB_H

#include "hash.h"

typedef struct {
    const char* key;
    int type;
    void* value;
} Symbol;

typedef Hash SymTab;

// defined in symbol_tab.c
extern SymTab* symtab;

void initSymbolTab();
int addSymbol(const char* key, int type, void* data);
Symbol* getSymbol(const char* key);
int getSymbolType(const char* key);
void* getSymbolData(const char* key);
void dumpSymTab();

#endif /* _COMPILER_SYMBOL_TAB_H */
