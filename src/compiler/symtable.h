#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

void addSym(const char* key, int type, void* value);
int symToType(const char* key);
void* symToVal(const char*key);
void dumpSymtab();

void pushContext(const char* name);
void popContext();

#endif
