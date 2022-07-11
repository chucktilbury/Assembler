
#include "opcodes.h"

const char* opToStr(OpCode op)
{
    return (op == OP_ABORT)? "ABORT":
            (op == OP_EXIT)? "EXIT":
            (op == OP_NOP)? "NOP":
            (op == OP_CALL)? "CALL":
            (op == OP_TRAP)? "TRAP":
            (op == OP_RETURN)? "RETURN":
            (op == OP_JMP)? "JMP":
            (op == OP_BR)? "BR":
            (op == OP_PUSH)? "PUSH":
            (op == OP_POP)? "POP":
            (op == OP_PEEK)? "PEEK":
            (op == OP_SIDX)? "SIDX":
            (op == OP_LOAD)? "LOAD":
            (op == OP_LOADI)? "LOADI":
            (op == OP_LOADR)? "LOADR":
            (op == OP_STORE)? "STORE":
            (op == OP_NOT)? "NOT":
            (op == OP_EQ)? "EQ":
            (op == OP_NEQ)? "NEQ":
            (op == OP_LEQ)? "LEQ":
            (op == OP_GEQ)? "GEQ":
            (op == OP_LESS)? "LESS":
            (op == OP_GTR)? "GTR":
            (op == OP_NEG)? "NEG":
            (op == OP_ADD)? "ADD":
            (op == OP_SUB)? "SUB":
            (op == OP_MUL)? "MUL":
            (op == OP_DIV)? "DIV":
            (op == OP_MOD)? "MOD": "UNKNOWN";
}
