#ifndef STR_UTILS_H
#define STR_UTILS_H

typedef struct {
    char* buf;
    uint32_t cap;
    uint32_t len;
} String;

String* createStr(const char* str);
String* createStrFmt(const char* fmt, ...);
void addStrChar(String* str, int ch);
void addStrStr(String* str, const char* strg);
void addStrFmt(String* str, const char* fmt, ...);
const char* formatStr(const char* str);
void stripStr(String* str);

#endif