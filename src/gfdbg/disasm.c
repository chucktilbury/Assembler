
#include "disasm.h"

void show_listing(FILE* fp)
{
    while(!instrIsEnd()) {
        uint8_t op;
        fprintf(fp, "%04d\t", getIndex());
        readInstObj(&op, sizeof(op));
        fprintf(fp, "%s\t", opToStr(op));

        switch(op) {
            case OP_EXIT:
            case OP_NOP:
            case OP_BREAK:
            case OP_RETURN:
                // no parameters, do nothing
                break;

            case OP_SIDX:
            case OP_PUSH:
            case OP_NOT:
            case OP_POP: {
                    // 1 register
                    uint8_t regs;
                    readInstObj(&regs, sizeof(regs));
                    fprintf(fp, "%s", regToStr(regs&0xF));
                }
                break;

            case OP_EQ:
            case OP_NEQ:
            case OP_LTE:
            case OP_GTE:
            case OP_LT:
            case OP_GT:
            case OP_NEG:
            case OP_LOADR: {
                    // 2 registers
                    uint8_t regs;
                    readInstObj(&regs, sizeof(regs));
                    fprintf(fp, "%s,%s", regToStr((regs>>4)&0xF),
                            regToStr(regs&0xF));
                }
                break;

            case OP_PEEK: {
                    uint8_t regs;
                    uint16_t offset;
                    readInstObj(&regs, sizeof(regs));
                    readInstObj(&offset, sizeof(offset));
                    fprintf(fp, "%s,%s,%d", regToStr((regs>>4)&0xF),
                            regToStr(regs&0xF), offset);
                }
                break;
            case OP_ADD:
            case OP_SUB:
            case OP_MUL:
            case OP_DIV:
            case OP_MOD: {
                    // 3 registers
                    uint16_t regs;
                    readInstObj(&regs, sizeof(regs));
                    fprintf(fp, "%s,%s,%s", regToStr((regs>>8)&0xF),
                            regToStr((regs>>4)&0xF), regToStr(regs&0xF));
                }
                break;

            case OP_TRAP: {
                    // one parameter given by a immediate uint16_t
                    uint16_t tno;
                    readInstObj(&tno, sizeof(tno));
                    fprintf(fp, "%d", tno);
                }
                break;

            case OP_LOAD: {
                    // register and a value table index
                    uint8_t reg;
                    Index idx;
                    readInstObj(&reg, sizeof(reg));
                    readInstObj(&idx, sizeof(idx));
                    fprintf(fp, "%s,idx:%d ", regToStr(reg&0xF), idx);
                    printVal(getValTab(idx));
                }
                break;

            case OP_LOADI: {
                    // register and a immediate value
                    uint8_t reg;
                    Value val;
                    readInstObj(&reg, sizeof(uint8_t));
                    readInstObj(&val, sizeof(Value));
                    fprintf(fp, "%s,", regToStr(reg&0xf));
                    printVal(&val);
                }
                break;

            case OP_STORE: {
                    // value table index and a register
                    Index idx;
                    uint8_t reg;
                    readInstObj(&idx, sizeof(idx));
                    readInstObj(&reg, sizeof(reg));
                    fprintf(fp, "idx:%d ", idx);
                    printVal(getValTab(idx));
                    fprintf(fp, ",%s", regToStr(reg&0x0F));
                }
                break;

            case OP_CALL:
            case OP_JMP:
            case OP_BR: {
                    // one variable given by an immediate address
                    uint32_t idx;
                    readInstObj(&idx, sizeof(idx));
                    fprintf(fp, "%d", idx);
                }
                break;
        }
        fprintf(fp, "\n");
    }
}

char fname_buf[FNAME_LEN];

void read_binary(const char* fname)
{
    FILE* fp = fopen(fname, "r");
    if(fp == NULL) {
        fprintf(stderr, "fatal error: cannot open input file: %s: %s\n", fname, strerror(errno));
        exit(1);
    }

    // output the file header.
    uint64_t magic;
    fread(&magic, sizeof(magic), 1, fp);
    if(magic != MAGIC_NUMBER) {
        fprintf(stderr, "fatal error: input file is not a virtual machine executable: %s\n", fname);
        exit(1);
    }

    memset((void*)fname_buf, 0, sizeof(fname_buf));
    fread((void*)fname_buf, sizeof(fname_buf), 1, fp);

    loadInstStream(fp);
    loadValTab(fp);
    //loadStrTab(fp);
}


int main(int argc, char** argv)
{
    _init_memory();
    initCmdLine(CL_FL_ONE, "Take a file that was created by the assembler and print\n"
                            "the raw disassembly, including the tables.");
    addStrParam("-o", "ofile", "output file name", "output.bin", CL_NONE);
    addNumParam("-v", "verbose", "verbosity number from 0 to 10", 0, CL_NONE);
    addCBwoParam("-h", "print the help information", showUseCmdLine, CL_NONE);
    parseCmdLine(argc, argv);

    resetCLFileList();
    read_binary(iterateCLFileList());

    show_listing(stdout);

    if(getNumParam("verbose") >= 5) {
        printf("file name from file: %s\n", fname_buf);
        dumpStrTab();
        dumpValTab();
    }
}
