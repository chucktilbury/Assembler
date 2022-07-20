#ifndef VMACHINE_H
#define VMACHINE_H

#include "common.h"

#include "runloop.h"
#include "vm_errors.h"
#include "val_stack.h"

bool getZflag();
void setZflag(bool val);

extern int verbosity;
extern bool trace;
#define TRACE(fmt, ...)  do { if(trace) fprintf(stderr, fmt, ## __VA_ARGS__ ); } while(false)
#define LOG(lev, fmt, ...)  do { \
     if(verbosity >= lev) \
        fprintf(stderr, "log: " fmt, ## __VA_ARGS__ );  \
        fprintf(stderr, "\n"); \
    } while(false)

#endif
