#ifndef VMACHINE_H
#define VMACHINE_H

#include "common.h"

#include "arith.h"
#include "compare.h"
#include "runloop.h"
#include "val_stack.h"

bool getZflag();
void setZflag(bool val);

#endif
