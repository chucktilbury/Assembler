
#include "vmachine.h"

#include "do_arith.h"
#include "do_compare.h"
#include "do_branch.h"
#include "do_stack.h"
#include "do_load.h"
#include "do_abort.h"
#include "do_trap.h"

static runloopHandler runloop_table[] = {
    [OP_ABORT] = doABORT,   // cause the VM to quit with an error found in a variable
    [OP_EXIT] = doEXIT,     // cause the VM to quit normally
    [OP_NOP] = doNOP,       // no operation
    [OP_CALL] = doCALL,     // call an absolute address stored in a variable
    [OP_TRAP] = doTRAP,     // call a pre-determined address for external functionality
    [OP_RETURN] = doRETURN, // return from a call or a trap
    [OP_JMP] = doJMP,       // unconditional jump to an absolute address stored in a variable
    [OP_BR] = doBR,         // conditional jump to an absolute address in a variable
    [OP_PUSH] = doPUSH,     // push the value on the value stack from a variable
    [OP_POP] = doPOP,       // pop the value from the stack and put it in a register
    [OP_LOAD] = doLOAD,     // Load a register from a variable.
    [OP_LOADI] = doLOADI,   // load a register from an immediate value.
    [OP_LOADR] = doLOADR,   // load a register from another register (copy)
    [OP_STORE] = doSTORE,   // Store a register into a variable.
    [OP_NOT] = doNOT,       // unary not conditional
    [OP_EQ] = doEQ,         // equal conditional
    [OP_NEQ] = doNEQ,       // not equal conditional
    [OP_LEQ] = doLEQ,       // less-or-equal conditional
    [OP_GEQ] = doGEQ,       // greater-or-equal conditional
    [OP_LESS] = doLESS,     // less than conditional
    [OP_GTR] = doGTR,       // greater than conditional
    [OP_NEG] = doNEG,       // unary arithmetic negation
    [OP_ADD] = doADD,       // arithmetic add
    [OP_SUB] = doSUB,       // arithmetic subtract
    [OP_MUL] = doMUL,       // arithmetic multiply
    [OP_DIV] = doDIV,       // arithmetic divide
    [OP_MOD] = doMOD,       // arithmetic modulo
    [OP_PEEK] = doPEEK,     // peek at the stack
    [OP_SIDX] = doSIDX,     // save the current top of stack
};

void runloop()
{
    bool finished = false;

    while(!finished) {

        uint8_t op;

        TRACE("%04d ", getIndex());
        readInstObj(&op, sizeof(op));
        TRACE("%s\t", opToStr(op));

        finished = runloop_table[op]();

        if(instrIsEnd())
            finished = true;

        if(getErrors())
            finished = true;

        TRACE("\n");
    }
}

