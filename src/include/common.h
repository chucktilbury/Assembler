#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

typedef uint32_t Index;

#include "memory.h"
#include "inst_buf.h"
#include "str_table.h"
#include "val_table.h"
#include "opcodes.h"
#include "registers.h"
#include "str_utils.h"
#include "errors.h"

#define MAGIC_NUMBER    ((uint64_t)(0x474F4C4446495348UL))
#define FNAME_LEN       64

#endif
