/**
 * @file asm.c
 * @brief This is a small driver that combines the cpp and gas programs so
 * that they can be run seamlessly. This is mostly about the command line
 * interface.
 */
#include <linux/limits.h> // for PATH_MAX = 4096 and NAME_MAX = 255
#include <stdarg.h>
#include "common.h"
#include "cmdline.h"

//static char path_buf[PATH_MAX];
static const char* defstr = NULL;
static const char* incastr = NULL;
static const char* incbstr = NULL;

// defined cmdline.c
void dump_cmd_line(cmd_line cptr);

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
static const char* actual_dir(const char* str)
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
        add_pp_incb(actual_dir(rt));
    }
}

int main(int argc, char** argv)
{
    _init_memory();

    defstr = _copy_str("");
    incastr = _copy_str("");
    incbstr = _copy_str("");

    get_runtime();

    cmd_line cl = create_cmd_line("This is the assembler");
    add_str_param(cl, "ifile", "-i", "input file name", "", CF_REQD);
    add_str_param(cl, "ofile", "-o", "output file name", "output.bin", CF_NONE);
    // add_str_param(cl, "opat", "-op", "output file path", "./", CF_NONE);
    add_toggle_param(cl, "keep", "-k", "keep intermediate file", false, CF_NONE);
    add_toggle_param(cl, "dry", "-n", "parse command line but do nothing", false, CF_NONE);
    add_num_param(cl, "verbose", "-v", "verbosity number from 0 to 10", 0, CF_NONE);
    add_callback_param(cl, "def", "-D", "preprocessor definition", add_pp_def, CF_NONE);
    add_callback_param(cl, "inc", "-I", "preprocessor include path", add_pp_incb, CF_NONE);
    add_callback_param(cl, "inc", "-J", "preprocessor include path", add_pp_inca, CF_NONE);

    parse_cmd_line(cl, argc, argv);
    int verbo = get_num_param(cl, "verbose");

    if(verbo > 9)
        dump_cmd_line(cl);

    const char* cmd = actual_dir(get_cmd(cl));
    const char* cmdpath = extract_path(cmd);
    const char* infn = actual_dir(get_str_param(cl, "ifile"));
    const char* path = extract_path(infn);
    const char* name = extract_name(infn);
    const char* ifn = append_str(path, "/%s.i", name);
    const char* outfn = append_str(path, "/%s", get_str_param(cl, "ofile"));

    if(verbo > 9) {
        printf("command: %s\n", cmd);
        printf("cmd path: %s\n", cmdpath);
        printf("cmd name: %s\n", extract_name(cmd));

        printf("infile: %s\n", infn);
        printf("infile path: %s\n", path);
        printf("infile name: %s\n", name);

        printf("temp name: %s\n", ifn);

        // if the full path is provided, then don't append
        printf("outfile name: %s\n\n", outfn);
    }

    const char* cmd1 = NULL;
    const char* cmd2 = NULL;
    cmd1 = append_str(NULL, "%s/cpp %s %s %s -o %s %s", cmdpath, defstr, incbstr, incastr, ifn, infn);
    cmd2 = append_str(NULL, "%s/gasm -v %d -i %s -o %s", cmdpath, verbo, ifn, outfn);
    if(verbo > 9) {
        printf("%s\n", cmd1);
        printf("%s\n\n", cmd2);
    }

    if(!get_toggle_param(cl, "dry")) {
        if(system(cmd1)) {
            fprintf(stderr, "error: preprocessing failed\n");
            return 1;
        }

        if(system(cmd2)) {
            fprintf(stderr, "error: assembler failed\n");
            return 1;
        }
    }

    if(!get_toggle_param(cl, "keep"))
        remove(ifn);

    _uninit_memory();
    return 0;
}
