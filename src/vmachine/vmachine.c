
#include "vmachine.h"
#include "cmdline.h"

static bool z_flag;

// virtual machine globals
Value registers[16];
const uint8_t* instr_buffer;
unsigned int instr_idx;
unsigned int instr_len;

int errors = 0;
int verbosity = 0;
bool trace = false;

void setZflag(bool val)
{
    z_flag = val;
}

bool getZflag()
{
    return z_flag;
}

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

    char fname_buf[FNAME_LEN];
    memset(fname_buf, 0, sizeof(fname_buf));
    fread(fname_buf, sizeof(fname_buf), 1, fp);

    loadInstStream(fp);
    loadValTab(fp);

    initValStack();
}

int main(int argc, char** argv)
{
    _init_memory();
    initCmdLine(CL_FL_ONE, "This is the virtual machine. It reads a binary\n"
                            "file that was created with the assembler and\n"
                            "runs it.\n");
    addTogParam("-t", "trace", "enable trace output", false, CL_NONE);
    addNumParam("-v", "verbose", "verbosity number from 0 to 10", 0, CL_NONE);
    addCBwoParam("-h", "print help information", showUseCmdLine, CL_NONE);
    parseCmdLine(argc, argv);

    verbosity = getNumParam("verbose");
    trace = getTogParam("trace");

    resetCLFileList();
    read_binary(iterateCLFileList());

    instr_buffer = getInstBuffer();
    instr_idx = 0;
    instr_len = getInstBufferLen();

    runloop();
}
