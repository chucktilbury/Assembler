#ifndef VMACHINE_H
#define VMACHINE_H

#include "common.h"

#include "runloop.h"
#include "val_stack.h"
#include "vm_errors.h"

extern const uint8_t* instr_buffer;
extern unsigned int instr_idx;
extern unsigned int instr_len;

#define READ_OBJ(v, t)                                \
    do {                                              \
        (v) = (t) * ((t*)(&instr_buffer[instr_idx])); \
        instr_idx += sizeof(t);                       \
    } while(false)

#define GET_IP() instr_idx
#define GET_IP_LEN() instr_len
#define ENDING_IP() (GET_IP() >= GET_IP_LEN())
#define SET_IP(idx)                                                                  \
    do {                                                                             \
        if(idx < GET_IP_LEN())                                                       \
            instr_idx = (idx);                                                       \
        else {                                                                       \
            fprintf(stderr, "runtime error: %d: instruction buffer overrun\n", idx); \
            exit(1);                                                                 \
        }                                                                            \
    } while(false)


bool getZflag();
void setZflag(bool val);

#ifdef ENA_TRACE
extern int verbosity;
extern bool trace;
#define TRACE(fmt, ...)                          \
    do {                                         \
        if(trace)                                \
            fprintf(stderr, fmt, ##__VA_ARGS__); \
    } while(false)
#else
#define TRACE(fmt, ...)
#endif

#define LOG(lev, fmt, ...)                               \
    do {                                                 \
        if(verbosity >= lev)                             \
            fprintf(stderr, "log: " fmt, ##__VA_ARGS__); \
        fprintf(stderr, "\n");                           \
    } while(false)

#define ONLY_REG(r) ((r)&0x000F)
#define DEST_REG(r) (((r)&0x0F00) >> 8)
#define LEFT_REG(r) (((r)&0x00F0) >> 4)
#define RIGHT_REG(r) ONLY_REG(r)

#endif
