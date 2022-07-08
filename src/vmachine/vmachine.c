
#include "vmachine.h"
#include "cmdline.h"

static bool z_flag;
Value registers[16];
int errors = 0;
int verbosity = 0;

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

    loadInstStream(fp);
    loadValBuf(fp);
    loadStrTab(fp);

    initValStack();
}


cmd_line cl;
int main(int argc, char** argv)
{
    _init_memory();
    cl = create_cmd_line("This is the dis-assembler");
    add_str_param(cl, "ifile", "-i", "input file name", "", CF_REQD);
    add_num_param(cl, "verbose", "-v", "verbosity number from 0 to 10", 0, CF_NONE);
    parse_cmd_line(cl, argc, argv);
    verbosity = get_num_param(cl, "verbose");

    read_binary(get_str_param(cl, "ifile"));

    runloop();
}
