
#include "gas.h"

typedef struct {
    char** list;
    uint32_t cap;
    uint32_t len;
} StrTab;

static StrTab stab;

void initStrTab()
{
    stab.cap = 0x01 << 3;
    stab.len = 0;
    stab.list = _alloc_ds_array(char*, stab.cap);
}

void saveStrTab(FILE* fp)
{
    fwrite(&stab.len, sizeof(stab.len), 1, fp);
    for(uint32_t idx = 0; idx < stab.len; idx++) {
        uint32_t len = strlen(stab.list[idx])+1;
        fwrite(&len, sizeof(len), 1, fp);
        fwrite(stab.list[idx], sizeof(char), len, fp);
    }
}

StrIdx addStr(const char* str)
{
    if(stab.len+1 > stab.cap) {
        stab.cap <<= 1;
        stab.list = _realloc_ds_array(stab.list, char*, stab.cap);
    }

    stab.list[stab.len] = _copy_str(str);
    uint32_t idx = stab.len;
    stab.len++;

    return idx;
}

const char* getStr(StrIdx idx)
{
    if(idx < stab.len)
        return stab.list[idx];
    else
        return NULL;
}

void dumpStrTab()
{
    printf("\n------- Dump String Table -------\n");
    for(uint32_t idx = 0; idx < stab.len; idx++) {
        printf("    idx: %d\t\"%s\"\n", idx, stab.list[idx]);
    }
    printf("------- End of dump -------\n");
}
