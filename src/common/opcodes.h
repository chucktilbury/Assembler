#ifndef OPCODES_H
#define OPCODES_H

typedef enum {
    OP_EXIT,    // cause the VM to quit normally
    OP_NOP,     // no operation
    OP_BREAK,   // no operation in VM

    // flow control instructions
    OP_CALL,    // call an absolute address stored in a variable
    OP_TRAP,    // call a pre-determined address for external functionality
    OP_RETURN,  // return from a call or a trap
    OP_JMP,     // unconditional jump to an absolute address stored in a variable
    OP_BR,      // conditional jump to an absolute address in a variable

    // data manipulation instructions
    OP_PUSH,    // push the register value on the value stack (change to variable)
    //OP_PUSHI,   // push an immediate value on the value stack
    //OP_PUSHR,   // push a register value on the value stack
    OP_POP,     // pop the value from the stack and put it in a register
    OP_PEEK,    // Stack access for function parameters.
    OP_SIDX,    // Store the stack index into a register

    OP_LOAD,    // Load a register from a variable.
    OP_LOADI,   // load a register from an immediate.
    OP_LOADR,   // load a register from another register (copy)
    OP_STORE,   // Store a register into a variable.
    //OP_STOREI,  // Store an immediate value into a variable.

    // comparison operators
    OP_NOT,     // unary not conditional
    OP_EQ,      // equal conditional
    OP_NEQ,     // not equal conditional
    OP_LTE,     // less-or-equal conditional
    OP_GTE,     // greater-or-equal conditional
    OP_LT,      // less than conditional
    OP_GT,      // greater than conditional

    // arithmetic operators
    OP_NEG,     // unary arithmetic negation
    OP_ADD,     // arithmetic add
    OP_SUB,     // arithmetic subtract
    OP_MUL,     // arithmetic multiply
    OP_DIV,     // arithmetic divide
    OP_MOD,     // arithmetic modulo

} OpCode;

const char* opToStr(OpCode op);

#endif
