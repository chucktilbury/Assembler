
#include "gas.h"

typedef struct {
    uint8_t* buf;
    uint32_t cap;
    uint32_t len;
} InstBuffer;

static InstBuffer ibuf;

static void copy_data(void* ptr, uint32_t size)
{
    if(ibuf.len+size > ibuf.cap) {
        while(ibuf.len+size > ibuf.cap)
            ibuf.cap <<= 1;
        ibuf.buf = _realloc_ds_array(ibuf.buf, uint8_t, ibuf.cap);
    }

    memcpy(&ibuf.buf[ibuf.len], ptr, size);
    ibuf.len += size;
}

void initInstStream()
{
    ibuf.cap = 1 << 3;
    ibuf.len = 0;
    ibuf.buf = _alloc_ds_array(uint8_t, ibuf.cap);
}

void saveInstStream(FILE* fp)
{
    fwrite(&ibuf.len, sizeof(ibuf.len), 1, fp);
    fwrite(ibuf.buf, sizeof(uint8_t), ibuf.len, fp);
}

void writeInstObj(void* ptr, uint32_t size)
{
    copy_data(ptr, size);
}

void writeInst8(uint8_t val)
{
    copy_data(&val, sizeof(val));
}

void writeInst16(uint16_t val)
{
    copy_data(&val, sizeof(val));
}

void writeInst32(uint32_t val)
{
    copy_data(&val, sizeof(val));
}

uint32_t getAddr()
{
    return ibuf.len;
}
