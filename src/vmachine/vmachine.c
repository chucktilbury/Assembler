
#include "vmachine.h"
#include "cmdline.h"

static bool z_flag;
Value registers[16];
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
    loadValBuf(fp);
    loadStrTab(fp);

    initValStack();
}


cmd_line cl;
int main(int argc, char** argv)
{
    _init_memory();
    cl = create_cmd_line("This is the dis-assembler");
    add_toggle_param(cl, "trace", "-t", "enable trace output", false, CF_NONE);
    add_num_param(cl, "verbose", "-v", "verbosity number from 0 to 10", 0, CF_NONE);
    parse_cmd_line(cl, argc, argv);
    verbosity = get_num_param(cl, "verbose");
    trace = get_toggle_param(cl, "trace");

    //read_binary(get_str_param(cl, "ifile"));

    reset_cmd_excess(cl);
    read_binary(iterate_cmd_excess(cl));
    runloop();
}
