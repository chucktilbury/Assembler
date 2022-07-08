#ifndef VMACHINE_H
#define VMACHINE_H

#include "common.h"

#include "runloop.h"
#include "vm_errors.h"
#include "val_stack.h"

bool getZflag();
void setZflag(bool val);

extern int verbosity;
#define TRACE(fmt, ...)  do { if(verbosity >= 5) fprintf(stdout, fmt, ## __VA_ARGS__ ); } while(false)

#endif
