#ifndef GAS_H
#define GAS_H

#include "common.h"

#include "val_table.h"
#include "asm_errors.h"
#include "scanner.h"
#include "memory.h"
#include "opcodes.h"
#include "ast.h"
#include "arith.h"
#include "inst_buf.h"
#include "post.h"
#include "parser.h"
#include "str_table.h"

//#include "symtab.h"


const char* preformat_str(const char* str);

#endif
