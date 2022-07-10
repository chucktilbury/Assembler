#include "common.h"
#include <stdarg.h>

String* createStr(const char* str)
{
    String* nstr = _alloc_ds(String);
    nstr->len = 0;

    if(str != NULL) {
        nstr->cap = 1;
        size_t len = strlen(str);
        while(nstr->cap < len+1)
            nstr->cap <<= 1;

        nstr->buf = _alloc_ds_array(char, nstr->cap);
        strcpy(nstr->buf, str);
        nstr->len = len;
    }
    else {
        nstr->cap = 0x01 << 3;
        nstr->buf = _alloc_ds_array(char, nstr->cap);
    }

    return nstr;
}

String* createStrFmt(const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* buf = _alloc(len+1);

    va_start(args, fmt);
    vsnprintf(buf, len+1, fmt, args);
    va_end(args);

    return createStr(buf);
}

void addStrChar(String* str, int ch)
{
    if(str->len+1 > str->cap) {
        str->cap <<= 1;
        str->buf = _realloc_ds_array(str->buf, char, str->cap);
    }

    str->buf[str->len] = (char)ch;
    str->len++;
}

void addStrStr(String* str, const char* strg)
{
    size_t len = strlen(strg);
    if(str->len+len+1 > str->cap) {
        while(str->cap < str->len+len+1)
            str->cap <<= 1;
        str->buf = _realloc_ds_array(str->buf, char, str->cap);
    }

    strcpy(&str->buf[str->len], strg);
    str->len += len;
}

void addStrFmt(String* str, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* buf = _alloc(len+1);

    va_start(args, fmt);
    vsnprintf(buf, len+1, fmt, args);
    va_end(args);

    addStrStr(str, buf);
}

const char* formatStr(const char* str)
{
    bool finished = false;
    String* s = createStr(NULL);
    String* tmp;
    int state = 0;
    int idx = 0;

    while(!finished) {
        switch(state) {
            case 0: {
                    // entry state. copying the string
                    switch(str[idx]) {
                        case '\0':
                            state = 255;
                            break;
                        case '{':
                            tmp = createStr(NULL);
                            state = 1;
                            break;
                        default:
                            addStrChar(s, str[idx]);
                            break;
                    }
                }
                break;

            case 1: {
                    // seen a '{' may or may not have a var name
                    switch(str[idx]) {
                        case '\0': // string ending
                            addStrChar(s, '{');
                            addStrStr(s, tmp->buf);
                            state = 255;
                            break;
                        case '{': // cannot be a variable
                            state = 2;
                            addStrChar(tmp, str[idx]);
                            break;
                        case '}': // tmp has a potential variable
                            state = 3;
                            break;
                        default:
                            if(!isdigit(str[idx]) && isspace(str[idx]))
                                state = 2;
                            addStrChar(tmp, str[idx]);
                            break;
                    }
                }
                break;

            case 2: {
                    // aborting the variable read
                    addStrChar(s, '{');
                    addStrStr(s, tmp->buf);
                    state = 0; // go back to copying chars
                }
                break;

            case 3: {
                    // have a potential variable
                    // if it's not a var name, then we need the original
                    Index sidx = strtol(tmp->buf, NULL, 10);
                    //printf("index: %d\n", sidx);
                    Value* val = getValBuf(sidx);
                    if(val->type != ERROR) {
                        //String* str = valToStr(val);
                        const char* str = valToStr(val);
                        addStrStr(s, str);
                    } else {
                        addStrFmt(s, "{%s}", tmp->buf);
                    }
                    idx--;
                    state = 0;
                }
                break;

            case 255: {
                    // ending normally
                    finished = true;
                }
        }
        idx++;
    }

    return s->buf;
}

void stripStr(String* str)
{
    int idx = 0;
    for(idx = 0; isspace(str->buf[idx]); idx++) {}
    if(idx > 0)
        memmove(str->buf, &str->buf[idx], strlen(&str->buf[idx]));
    for(idx = strlen(str->buf)-1; idx > 0 && isspace(str->buf[idx]); idx--) {}
    idx++;
    str->buf[idx] = '\0';
    str->len = strlen(str->buf);
}
