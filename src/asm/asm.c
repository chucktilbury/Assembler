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
static const char* input_path_str = NULL;
static const char* output_path_str = NULL;
static const char* data_name = NULL;

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

static void create_input_list(cmd_line cl)
{
    reset_cmd_excess(cl);
    for(char* str = iterate_cmd_excess(cl); str != NULL; str = iterate_cmd_excess(cl)) {
        pproc_line = append_str(pproc_line, " %s", find_file(str));

        Path* p = _alloc_ds(Path);
        p->str = _copy_str(find_file(str));
        p->next = NULL;

        if(input_path == NULL) {
            input_list = input_list_end = p;
        }
        else {
            input_list_end->next = p;
            input_list_end = p;
        }
    }
}

static void print_path(Path* path)
{
    for(Path* p = path; p != NULL; p = p->next)
        printf("  %s\n", p->str);
}

int main(int argc, char** argv)
{
    create_exec_path();
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
    create_input_list(cl);
    exec_path_str = extract_path(actual_exec_dir(argv[0]));
    output_path_str = find_input_dir(get_str_param(cl, "opat"));


    printf("-------\n");
    print_path(input_path);
    printf("-------\n");
    printf("\"%s\"\n", pproc_line);
    return 0;
}





#if 0

//static char path_buf[PATH_MAX];
static const char* defstr = NULL;
static const char* incastr = NULL;
static const char* incbstr = NULL;

/**
 * @brief Append the formatted string to str and return a pointer to it.
 * Use free().
 *
 * @param str
 * @param fmt
 * @param ...
 *
 * @return const char*
 */
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

/**
 * @brief Assume the last thing after a '/' is a name and everything in
 * front of it is path. If there are no '/' in the string then return an
 * empty string.
 *
 * @param str
 *
 * @return const char*
 */
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

/**
 * @brief The given string is assumed to be a complete path name to a file
 * that has an extention. This function extracts the raw name without any
 * extention and returns it as an allocated string.
 *
 * @param str
 *
 * @return const char*
 */
static const char* extract_name(const char* str)
{
    char* ptr = _copy_str(str);
    char* start = strrchr(ptr, '/');
    if(start != NULL && *(start+1) == '\0') {
        // last character was a '/'
        fprintf(stderr, "error: invalid file name: %s\n", str);
        exit(1);
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

/**
 * @brief Return an allocated pointer to the full path given by the input
 * string. If realpath() can't find it, then look in the path. If it still
 * can't be found, then just return the name as it was given.
 *
 * @param str
 *
 * @return const char*
 */
static const char* actual_exec_dir(const char* str)
{
    char* real = realpath(str, NULL);
    if(real != NULL)
        return real;
    else {
        real = _copy_str(getenv("PATH")); // strtok destroys the string
        for(char* tmp = strtok(real, ":"); tmp != NULL; tmp = strtok(NULL, ":")) {
            char buf[1024];
            strncpy(buf, tmp, sizeof(buf));
            strcat(buf, "/");
            strcat(buf, str); // TODO: buffer discipline
            if(!access(buf, R_OK|X_OK))
                return _copy_str(buf);
        }
    }

    return str;
}

/**
 * @brief Command line call back appends a "-D" command line option to the
 * string for the preprocessor command line.
 *
 * @param str
 *
 */
static void add_pp_def(const char* str)
{
    defstr = append_str(defstr, " -D %s", str);
}

/**
 * @brief Command line call back appends a "-I" command line option to the
 * string for the preprocessor command line.
 *
 * @param str
 *
 */
static void add_pp_incb(const char* str)
{
    incbstr = append_str(incbstr, " -I %s", str);
}

/**
 * @brief Command line call back appends a "-J" command line option to the
 * string for the preprocessor command line.
 *
 * @param str
 *
 */
static void add_pp_inca(const char* str)
{
    incastr = append_str(incastr, " -J %s", str);
}

static void get_runtime()
{
    char* rt = getenv("_RUNTIME");
    if(rt != NULL) {
        add_pp_incb(actual_exec_dir(rt));
    }
}

int main(int argc, char** argv)
{
    _init_memory();

    defstr = _copy_str("");
    incastr = _copy_str("");
    incbstr = _copy_str("");

    get_runtime();

    cmd_line cl = create_cmd_line("This is the assembler driver program.\n"\
                                "It coordinates the operation of the preprocessor,\n"\
                                "the assembler, and the virtual machine.");

    add_str_param(cl, "ofile", "-o", "output file name", "output.bin", CF_NONE);
    add_str_param(cl, "opat", "-p", "output file path", NULL, CF_NONE);
    //add_str_param(cl, "ipat", "-i", "input file path", NULL, CF_NONE);
    add_toggle_param(cl, "run", "-r", "run the result in the VM", false, CF_NONE);
    add_toggle_param(cl, "dbg", "-d", "debug the result in the debugger", false, CF_NONE);
    add_toggle_param(cl, "keep", "-k", "keep intermediate file(s)", false, CF_NONE);
    //add_toggle_param(cl, "dry", "-n", "parse command line but do nothing", false, CF_NONE);
    add_num_param(cl, "verbose", "-v", "verbosity level", 0, CF_NONE);
    add_callback_param(cl, "def", "-D", "preprocessor definition", add_pp_def, CF_NONE);
    add_callback_param(cl, "incb", "-I", "preprocessor include path", add_pp_incb, CF_NONE);
    add_callback_param(cl, "inca", "-J", "preprocessor include path", add_pp_inca, CF_NONE);

    parse_cmd_line(cl, argc, argv);
    int verbo = get_num_param(cl, "verbose");

    if(verbo > 15)
        dump_cmd_line(cl);

    // const char* cmd = actual_exec_dir(get_cmd(cl));
    // const char* cmdpath = extract_path(cmd);
    // reset_cmd_excess(cl);
    // const char* infn = actual_exec_dir(iterate_cmd_excess(cl)); //get_str_param(cl, "ifile"));
    // const char* path = extract_path(infn);
    // const char* name = extract_name(infn);
    // const char* ifn = append_str(path, "/%s.i", name);
    // const char* outfn = append_str(path, "/%s", get_str_param(cl, "ofile"));

    // if(verbo > 9000) {
    //     printf("command: %s\n", cmd);
    //     printf("cmd path: %s\n", cmdpath);
    //     printf("cmd name: %s\n", extract_name(cmd));

    //     printf("infile: %s\n", infn);
    //     printf("infile path: %s\n", path);
    //     printf("infile name: %s\n", name);

    //     printf("temp name: %s\n", ifn);

    //     // if the full path is provided, then don't append
    //     printf("outfile name: %s\n\n", outfn);
    // }

    //for() {}
    const char* cmd1 = NULL;
    const char* cmd2 = NULL;
    //cmd1 = append_str(NULL, "%s/cpp %s %s %s -o %s %s", cmdpath, defstr, incbstr, incastr, ifn, infn);
    //cmd2 = append_str(NULL, "%s/gasm -v %d -i %s -o %s", cmdpath, verbo, ifn, outfn);
    //cmd2 = append_str(NULL, "%s/gasm -v %d -o %s %s", cmdpath, verbo, outfn, ifn);
    // if(verbo > 9) {
    //     printf("%s\n", cmd1);
    //     printf("%s\n\n", cmd2);
    // }

    const char* cmdpath = extract_path(actual_exec_dir(get_cmd(cl)));

    reset_cmd_excess(cl);
    for(const char* str = iterate_cmd_excess(cl); str != NULL; str = iterate_cmd_excess(cl)) {

        const char* path = extract_path(str);
        const char* tmp_fn = append_str(path, "/%s.i", extract_name(str));
        cmd1 = append_str(NULL, "%s/cpp %s %s %s -o %s %s",
                            cmdpath, defstr, incbstr, incastr, tmp_fn, str);

        //if(verbo > 10)
            printf("\n%s", cmd1);

        // if(system(cmd1)) {
        //     fprintf(stderr, "error: preprocessing run failed\n");
        //     return 1;
        // }
    }

    reset_cmd_excess(cl);
    for(const char* str = iterate_cmd_excess(cl); str != NULL; str = iterate_cmd_excess(cl)) {

        const char* path = extract_path(str);
        const char* out_fn = append_str(path, "/%s", get_str_param(cl, "ofile"));
        cmd2 = append_str(NULL, "%s/gasm -v %d -o %s %s", cmdpath, verbo, out_fn, tmp_fn);

        //if(verbo > 10)
            printf("\n%s", cmd2);

        // if(system(cmd2)) {
        //     fprintf(stderr, "error: assembler run failed\n");
        //     return 1;
        // }

        // if(!get_toggle_param(cl, "keep"))
        //     remove(out_fn);
    }

    _uninit_memory();
    return 0;
}
#endif