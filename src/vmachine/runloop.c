
#include "vmachine.h"

#include "do_arith.c"
#include "do_compare.c"
#include "do_branch.c"
#include "do_stack.c"
#include "do_load.c"
#include "do_misc.c"

void runloop()
{
    bool finished = false;

    while(!finished) {

        uint8_t op;

        TRACE("%04d ", getIndex());
        READ_OBJ(op, uint8_t);
        TRACE("%s\t", opToStr(op));

        switch(op) {
            case OP_EXIT:   finished = doEXIT();   break;
            case OP_NOP:    finished = doNOP();    break;
            case OP_CALL:   finished = doCALL();   break;
            case OP_TRAP:   finished = doTRAP();   break;
            case OP_RETURN: finished = doRETURN(); break;
            case OP_JMP:    finished = doJMP();    break;
            case OP_BR:     finished = doBR();     break;
            case OP_PUSH:   finished = doPUSH();   break;
            case OP_POP:    finished = doPOP();    break;
            case OP_PEEK:   finished = doPEEK();   break;
            case OP_SIDX:   finished = doSIDX();   break;
            case OP_LOAD:   finished = doLOAD();   break;
            case OP_LOADI:  finished = doLOADI();  break;
            case OP_LOADR:  finished = doLOADR();  break;
            case OP_STORE:  finished = doSTORE();  break;
            case OP_NOT:    finished = doNOT();    break;
            case OP_EQ:     finished = doEQ();     break;
            case OP_NEQ:    finished = doNEQ();    break;
            case OP_LTE:    finished = doLTE();    break;
            case OP_GTE:    finished = doGTE();    break;
            case OP_LT:     finished = doLT();     break;
            case OP_GT:     finished = doGT();     break;
            case OP_NEG:    finished = doNEG();    break;
            case OP_ADD:    finished = doADD();    break;
            case OP_SUB:    finished = doSUB();    break;
            case OP_MUL:    finished = doMUL();    break;
            case OP_DIV:    finished = doDIV();    break;
            case OP_MOD:    finished = doMOD();    break;
            default:
               runtimeError("unknown instruction code: %d", op);
                exit(1);
        }

        if(ENDING_IP())
            finished = true;    // this is a programming error in the input

        if(getErrors())
            finished = true;

        TRACE("\n");
    }
}

