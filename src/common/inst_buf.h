#ifndef _COMMON_INST_BUF_H
#define _COMMON_INST_BUF_H


void initInstStream();
void saveInstStream(FILE* fp);
void loadInstStream(FILE* fp);
void writeInstObj(void* ptr, uint32_t size);
void readInstObj(void* ptr, uint32_t size);
void writeInst8(uint8_t val);
void writeInst16(uint16_t val);
void writeInst32(uint32_t val);
uint32_t getAddr();
uint32_t getIndex();
bool setIndex(uint32_t idx);
bool instrIsEnd();

const uint8_t* getInstBuffer();
unsigned int getInstBufferLen();

#endif /* _COMMON_INST_BUF_H */
