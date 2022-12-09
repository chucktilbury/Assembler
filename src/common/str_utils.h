#ifndef _COMMON_STR_UTILS_H
#define _COMMON_STR_UTILS_H

typedef struct {
    char* buf;
    uint32_t cap;
    uint32_t len;
} String;

typedef struct {
    String** list;
    uint32_t cap;
    uint32_t len;
    uint32_t index;
} StrList;

String* createStr(const char* str);
String* createStrFmt(const char* fmt, ...);
void addStrChar(String* str, int ch);
void addStrStr(String* str, const char* strg);
void addStrFmt(String* str, const char* fmt, ...);
const char* formatStr(const char* str);
void stripStr(String* str);

StrList* createStrList();
void addStrList(StrList* lst, String* str);
String* getStrList(StrList* lst, uint32_t idx);
String* resetStrList(StrList* lst);
String* iterateStrList(StrList* lst);

#endif /* _COMMON_STR_UTILS_H */
