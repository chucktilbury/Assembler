/**
 * @file asm.c
 * @brief This is a small driver that combines the cpp, gas, vm programs so
 * that they can be run seamlessly. This is mostly about the command line
 * interface.
 *
 * Command parameters
 *  -h  help text (created automatically)
 *  -r  Preprocess, assemble, and run the file given on the command line.
 *  -d  Preprocess, assemble, and debug the file given on the command line. (not implemented)
 *  -k  Keep the intermediate files. Normally deleted.
 *  -v  Set the verbosity level from nothing to ridiculous.
 *  -o  Output file. (if -r && ! -k, then delete when complete)
 *  -p  Where to place all output and intermediate files. If this is not provided, then the
 *      location of the input file is used.
 *  -D  Preprocessor "defines". Multiple instances can be provided, and/or a list separated
 *      by a ':' character can be provided.
 *  -I  Prepend an entry to the input search path for all inputs, not just preprocessor.
 *  -J  Append an entry to the input search path for all inputs, not just preprocessor.
 */
#include <linux/limits.h> // for PATH_MAX = 4096 and NAME_MAX = 255
#include <stdarg.h>
#include "common.h"
#include "cmdline.h"
//#include "str_utils.h"

typedef struct _str_path {
    const char* str;
    struct _str_path* next;
} Path;

// defined cmdline.c
void dump_cmd_line(cmd_line cptr);
static Path* input_path = NULL;
static Path* input_path_end = NULL;

static Path* input_list = NULL;
static Path* input_list_end = NULL;

static Path* exec_path = NULL;
static Path* exec_path_end = NULL;

static const char* pproc_line = NULL;
static const char* exec_path_str = NULL;
//static const char* input_path_str = NULL;
static const char* output_path_str = NULL;
//static const char* data_name = NULL;

// Append the string or create it.
static const char* append_str(const char* str, const char* fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    size_t nlen = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* ptr;
    if(str != NULL) {
        size_t olen = strlen(str);
        ptr = _alloc(nlen+olen+1);

        strcpy(ptr, str);
        va_start(args, fmt);
        vsprintf(&ptr[olen], fmt, args);
        va_end(args);
    }
    else {
        ptr = _alloc(nlen+1);

        va_start(args, fmt);
        vsprintf(ptr, fmt, args);
        va_end(args);
    }

    return ptr;
}

// Return the path, without the name. If no path is found, then return
// an empty string.
static const char* extract_path(const char* str)
{
    char* ptr = _copy_str(str);
    char* tmp = strrchr(ptr, '/');
    if(tmp != NULL)
        *tmp = '\0';
    else
        ptr[0] = '\0';

    return ptr;
}

// Return the name part of the path. This has the path stripped and the
// the last '.' stripped. If no name is found, then return an empty string.
static const char* extract_name(const char* str)
{
    char* ptr = _copy_str(str);
    char* start = strrchr(ptr, '/');
    if(start != NULL && *(start+1) == '\0') {
        // last character was a '/'
        ptr[0] = '\0';
        return ptr;
    }

    if(start != NULL) {
        start++;

        char* end = strrchr(start, '.');
        if(end != NULL)
            *end = '\0';

        return start;
    }
    else {
        char* end = strrchr(ptr, '.');
        if(end != NULL)
            *end = '\0';

        return ptr;
    }
}

// Return the full path to this program, including the command name.
static const char* actual_exec_dir(const char* str)
{
    char* real = realpath(str, NULL);
    if(real != NULL)
        return real;
    else {
        for(Path* pth = exec_path; pth != NULL; pth = pth->next) {
            char buf[PATH_MAX];
            strncpy(buf, pth->str, sizeof(buf));
            strcat(buf, "/");
            strcat(buf, str); // TODO: buffer discipline
            if(!access(buf, R_OK|X_OK))
                return _copy_str(buf);
        }
    }

    return str;
}

// verify that a file exists and return the full path to it.
static const char* find_file(const char* str)
{
    char* real = realpath(str, NULL);
    if(real != NULL)
        return real;
    else {
        for(Path* pth = input_path; pth != NULL; pth = pth->next) {
            char buf[PATH_MAX];
            strncpy(buf, pth->str, sizeof(buf));
            strcat(buf, "/");
            strcat(buf, str); // TODO: buffer discipline
            if(!access(buf, F_OK))
                return _copy_str(buf);
        }
    }

    // the file could not be located.
    return NULL;
}

static void add_pp_def(const char* s)
{
    pproc_line = append_str(pproc_line, " -D %s", s);
}

static const char* find_input_dir(const char* str)
{
    char* real = realpath(str, NULL);
    if(real != NULL)
        return real;
    else
        return str;
}

static void add_pp_incb(const char* s)
{
    char* tmp = _copy_str(s);
    for(const char* str = strtok(tmp, ":"); str != NULL; str = strtok(NULL, ":")) {
        pproc_line = append_str(pproc_line, " -I %s", find_input_dir(str));

        Path* p = _alloc_ds(Path);
        p->str = _copy_str(find_input_dir(str));
        p->next = NULL;

        if(input_path == NULL) {
            input_path = input_path_end = p;
        }
        else {
            p->next = input_path;
            input_path = p;
        }
    }
}

static void add_pp_inca(const char* s)
{
    char* tmp = _copy_str(s);
    for(const char* str = strtok(tmp, ":"); str != NULL; str = strtok(NULL, ":")) {
        pproc_line = append_str(pproc_line, " -J %s", find_input_dir(str));

        Path* p = _alloc_ds(Path);
        p->str = _copy_str(find_input_dir(str));
        p->next = NULL;

        if(input_path == NULL) {
            input_path = input_path_end = p;
        }
        else {
            input_path_end->next = p;
            input_path_end = p;
        }
    }
}

static void create_exec_path()
{
    char* raw_path = _copy_str(getenv("PATH"));
    for(const char* str = strtok(raw_path, ":"); str != NULL; str = strtok(NULL, ":")) {
        Path* p = _alloc_ds(Path);
        p->str = _copy_str(str);
        p->next = NULL;

        if(exec_path == NULL) {
            exec_path = exec_path_end = p;
        }
        else {
            exec_path_end->next = p;
            exec_path_end = p;
        }
    }
}

static const char* create_input_list(cmd_line cl)
{
    const char* retv = NULL;
    reset_cmd_excess(cl);
    for(char* str = iterate_cmd_excess(cl); str != NULL; str = iterate_cmd_excess(cl)) {
        char* found = (char*)find_file(str);
        if(found != NULL) {
            pproc_line = append_str(pproc_line, " %s", found);
            retv = extract_name(found);

            Path* p = _alloc_ds(Path);
            p->str = found; //_copy_str(find_file(str));
            p->next = NULL;

            if(input_list == NULL) {
                input_list = input_list_end = p;
            }
            else {
                fprintf(stderr, "cmd_err: extra input files. use exactly one and include any others.\n");
                cmd_use(cl);
                // input_list_end->next = p;
                // input_list_end = p;
            }
        }
        else {
            fprintf(stderr, "cmd_err: input file: %s: cannot be found\n", str);
            cmd_use(cl);
        }
    }
    return retv;
}

static void print_path(Path* path)
{
    for(Path* p = path; p != NULL; p = p->next)
        printf("    %s\n", p->str);
}

static void find_runtime()
{
    char* env = getenv("_RUNTIME");
    if(env != NULL)
        add_pp_incb(env);
}

int main(int argc, char** argv)
{
    create_exec_path();
    add_pp_incb("./");

    cmd_line cl = create_cmd_line("This is the assembler driver program.\n"\
                                "It coordinates the operation of the preprocessor,\n"\
                                "the assembler, and the virtual machine.");

    add_str_param(cl, "ofile", "-o", "output file name", "output.bin", CF_NONE);
    add_str_param(cl, "opat", "-p", "output file path", "./", CF_NONE);
    add_toggle_param(cl, "run", "-r", "run the result in the VM", false, CF_NONE);
    add_toggle_param(cl, "dbg", "-d", "debug the result in the debugger", false, CF_NONE);
    add_toggle_param(cl, "keep", "-k", "keep intermediate file(s)", false, CF_NONE);
    add_num_param(cl, "verbose", "-v", "verbosity level", 0, CF_NONE);
    add_callback_param(cl, "def", "-D", "preprocessor definition", add_pp_def, CF_NONE);
    add_callback_param(cl, "incb", "-I", "preprocessor include path", add_pp_incb, CF_NONE);
    add_callback_param(cl, "inca", "-J", "preprocessor include path", add_pp_inca, CF_NONE);

    parse_cmd_line(cl, argc, argv);


    int verbo = get_num_param(cl, "verbose");
    const char* base_file_name = create_input_list(cl);
    exec_path_str = extract_path(actual_exec_dir(argv[0]));
    output_path_str = find_input_dir(get_str_param(cl, "opat"));
    const char* ifn = append_str(base_file_name, ".i");
    const char* ofn = append_str(base_file_name, ".bin");
    find_runtime();

    const char* pp_cmd_line = append_str(NULL, "%s/cpp -o %s/%s %s",
                    exec_path_str, output_path_str, ifn, pproc_line);
    const char* asm_cmd_line = append_str(NULL, "%s/gasm -o %s/%s -v %d %s/%s",
                    exec_path_str, output_path_str, ofn, verbo, output_path_str, ifn);

    const char* vm_cmd_line;
    if(verbo > 5)
        vm_cmd_line = append_str(NULL, "%s/vmachine %s/%s -t -v %d",
                    exec_path_str, output_path_str, ofn, verbo);
    else
        vm_cmd_line = append_str(NULL, "%s/vmachine %s/%s -v %d",
                    exec_path_str, output_path_str, ofn, verbo);


    if(verbo > 10) {
        if(verbo > 20)
            dump_cmd_line(cl);
        printf("\nexecutable path:\n");
        print_path(exec_path);
        printf("\ninput path:\n");
        print_path(input_path);
        printf("\npreprocessor command line:\n");
        printf("    %s\n", pp_cmd_line);
        printf("\nassembler command line:\n");
        printf("    %s\n", asm_cmd_line);
        printf("\nvirtual machine command line:\n");
        printf("    %s\n", vm_cmd_line);
    }

    int retv = 0;
#if 1
    retv = system(pp_cmd_line);

    if(retv == 0)
        retv = system(asm_cmd_line);
    else {
        fprintf(stderr, "error: preprocessor failed\n");
        exit(retv);
    }

    if(retv == 0) {
        if(get_toggle_param(cl, "run"))
            retv = system(vm_cmd_line);
    }
    else {
        fprintf(stderr, "error: assembler failed\n");
        exit(retv);
    }

    if(retv != 0) {
        fprintf(stderr, "error: virtual machine failed\n");
        exit(retv);
    }

    if(!get_toggle_param(cl, "keep")) {
        remove(ifn);
        if(get_toggle_param(cl, "run"))
            remove(ofn);
    }
#endif
    return retv;
}

