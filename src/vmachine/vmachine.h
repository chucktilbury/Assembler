#ifndef VMACHINE_H
#define VMACHINE_H

#include "common.h"

#include "runloop.h"
#include "vm_errors.h"
#include "val_stack.h"

bool getZflag();
void setZflag(bool val);

#endif
