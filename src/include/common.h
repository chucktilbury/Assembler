#ifndef COMMON_H
#define COMMON_H

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef uint32_t Index;

#include "errors.h"
#include "inst_buf.h"
#include "memory.h"
#include "opcodes.h"
#include "registers.h"
#include "str_table.h"
#include "str_utils.h"
#include "val_table.h"

#define MAGIC_NUMBER ((uint64_t)(0x474F4C4446495348UL))
#define FNAME_LEN 64

#endif
