#ifndef POST_H
#define POST_H

void doPostProcess(Module* mod);
void dumpPostTables();

void addValTab(const char* key, Value* val, ValIdx idx);
//ValTab* findValTab(const char* key);
ValIdx findValTabIdx(const char* key);

#endif
