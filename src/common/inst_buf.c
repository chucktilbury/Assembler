
#include "common.h"

typedef struct {
    uint8_t* buf;
    uint32_t cap;
    uint32_t len;
    uint32_t idx;
} InstBuffer;

static InstBuffer ibuf;

static inline void write_data(void* ptr, uint32_t size)
{
    if(ibuf.len+size > ibuf.cap) {
        while(ibuf.len+size > ibuf.cap)
            ibuf.cap <<= 1;
        ibuf.buf = _realloc_ds_array(ibuf.buf, uint8_t, ibuf.cap);
    }

    memcpy(&ibuf.buf[ibuf.len], ptr, size);
    ibuf.len += size;
}

static inline void read_data(void* ptr, uint32_t size)
{
    if(ibuf.idx+size <= ibuf.len) {
        memcpy(ptr, &ibuf.buf[ibuf.idx], size);
        ibuf.idx += size;
    }
    else {
        fprintf(stderr, "fatal error: attempt to read past the end of the instruction buffer: %d\n", ibuf.idx+size);
        exit(1);
    }
}

void initInstStream()
{
    ibuf.cap = 1 << 3;
    ibuf.len = 0;
    ibuf.buf = _alloc_ds_array(uint8_t, ibuf.cap);
}

void loadInstStream(FILE* fp)
{
    ibuf.cap = 1;
    ibuf.len = 0;

    fread(&ibuf.len, sizeof(ibuf.len), 1, fp);
    //printf("read instr len: %d\n", ibuf.len);
    while(ibuf.len+1 > ibuf.cap)
        ibuf.cap <<= 1;

    ibuf.buf = _alloc_ds_array(uint8_t, ibuf.cap);
    fread(ibuf.buf, sizeof(uint8_t), ibuf.len, fp);
}

void saveInstStream(FILE* fp)
{
    //printf("write instr len: %d\n", ibuf.len);
    fwrite(&ibuf.len, sizeof(ibuf.len), 1, fp);
    fwrite(ibuf.buf, sizeof(uint8_t), ibuf.len, fp);
}

void writeInstObj(void* ptr, uint32_t size)
{
    write_data(ptr, size);
}

void readInstObj(void* ptr, uint32_t size)
{
    read_data(ptr, size);
}

void writeInst8(uint8_t val)
{
    write_data(&val, sizeof(uint8_t));
}

void writeInst16(uint16_t val)
{
    write_data(&val, sizeof(uint16_t));
}

void writeInst32(uint32_t val)
{
    write_data(&val, sizeof(uint32_t));
}

uint32_t getAddr()
{
    return ibuf.len;
}

uint32_t getIndex()
{
    return ibuf.idx;
}

bool setIndex(uint32_t idx)
{
    if(idx < ibuf.len) {
        ibuf.idx = idx;
        return false;
    }
    else {
        fprintf(stderr, "runtime error: invalid goto address: %d(%d)", idx, ibuf.len);
        return true;
    }
    //return true;
}

bool instrIsEnd()
{
    return (ibuf.idx >= ibuf.len);
}

const uint8_t* getInstBuffer()
{
    return ibuf.buf;
}

uint32_t getInstBufferLen()
{
    return ibuf.len;
}