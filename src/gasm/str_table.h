#ifndef STR_TABLE_H
#define STR_TABLE_H

typedef uint32_t StrIdx;

void initStrTab();
void saveStrTab(FILE* fp);
StrIdx addStr(const char* str);
const char* getStr(StrIdx idx);
void dumpStrTab();

#endif
