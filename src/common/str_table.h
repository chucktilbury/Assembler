#ifndef _COMMON_STR_TABLE_H
#define _COMMON_STR_TABLE_H

typedef Index StrIdx;

void initStrTab();
void saveStrTab(FILE* fp);
void loadStrTab(FILE* fp);
StrIdx addStr(const char* str);
const char* getStr(StrIdx idx);
void replaceStr(StrIdx idx, const char* str);
void dumpStrTab();

#endif /* _COMMON_STR_TABLE_H */
