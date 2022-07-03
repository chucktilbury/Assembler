#ifndef INST_BUFFER_H
#define INST_BUFFER_H


void initInstStream();
void saveInstStream(FILE* fp);
void writeInstObj(void* ptr, uint32_t size);
void writeInst8(uint8_t val);
void writeInst16(uint16_t val);
void writeInst32(uint32_t val);
uint32_t getAddr();

#endif
