#ifndef _ASSEMBLER_POST_H
#define _ASSEMBLER_POST_H

void doPostProcess(Module* mod);
void dumpPostTables();

void addSymVal(const char* key, Value* val, ValIdx idx);
// ValTab* findValTab(const char* key);
ValIdx findValTabIdx(const char* key);

#endif /* _ASSEMBLER_POST_H */
