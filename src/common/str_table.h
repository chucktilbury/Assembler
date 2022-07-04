#ifndef STR_TABLE_H
#define STR_TABLE_H

typedef Index StrIdx;

void initStrTab();
void saveStrTab(FILE* fp);
void loadStrTab(FILE* fp);
StrIdx addStr(const char* str);
const char* getStr(StrIdx idx);
void dumpStrTab();

#endif
