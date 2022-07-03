/*
 * Generic command line that sort of works like a simplified Python command
 * line parser.
 *  1.  create_cmd_line()
 *  2.  add_xxx_param() for each parameter to be parsed.
 *  3.  parse_cmd_line() to read the command line and parse it into the
 *      parameters.
 *  4.  get_xxx_param() to read the values that were parsed from the command
 *      line.
 *
 * If an invalid parameter or some other command line error happens then text
 * is produced on stderr to describe the command parameters that have been
 * specified.
 */
#ifndef CMDLINE_H
#define CMDLINE_H

// TODO: Add logic to allow toggle params to be combined into a single clump.

// opaque handle for command line parser
typedef void* cmd_line;
typedef void (*cmd_callback)(const char* ptr);

typedef enum {
    CF_NONE = 0x00,
    CF_REQD = 0x01,
    CF_TOGGLE = 0x02,
    CF_PRESENT = 0x04,
} cmd_flags_t;

typedef enum {
    CT_TOGGLE,
    CT_BOOL,
    CT_NUM,
    CT_STR,
    CT_CALLBACK,
} cmd_type_t;

// create the command line parser
cmd_line create_cmd_line(const char* docs);

// add a string command line parameter to the parser
void add_str_param(cmd_line cptr,   // pointer to the command line data structure
                const char* name,   // friendly name to use to look it up
                const char* parm,   // the parameter to recognize. (i.e. "-n")
                const char* help,   // help string
                const char* val,    // default value
                cmd_flags_t flags); // flags, such as required

// add a boolean command line parameter to the parser
void add_bool_param(cmd_line cptr,  // pointer to the command line data structure
                const char* name,   // friendly name to use to look it up
                const char* parm,   // the parameter to recognize. (i.e. "-n")
                const char* help,   // help string
                bool val,           // default value
                cmd_flags_t flags); // flags, such as required

// add a string command line parameter to the parser
void add_num_param(cmd_line cptr,   // pointer to the command line data structure
                const char* name,   // friendly name to use to look it up
                const char* parm,   // the parameter to recognize. (i.e. "-n")
                const char* help,   // help string
                int val,            // default value
                cmd_flags_t flags); // flags, such as required

void add_toggle_param(cmd_line cptr,  // pointer to the command line data structure
                const char* name,   // friendly name to use to look it up
                const char* parm,   // the parameter to recognize. (i.e. "-n")
                const char* help,   // help string
                bool val,           // default value
                cmd_flags_t flags); // flags, such as required

void add_callback_param(cmd_line cptr,  // pointer to the command line data structure
                const char* name,   // friendly name to use to look it up
                const char* parm,   // the parameter to recognize. (i.e. "-n")
                const char* help,   // help string
                cmd_callback val,   // func to call when this param is seen
                cmd_flags_t flags); // flags, such as required

// get the value associated with a command line parameter
const char* get_str_param(cmd_line cptr,// pointer to the command line data structure
                const char* name);  // the parameter to recognize. (i.e. "-n")

bool get_bool_param(cmd_line cptr,  // pointer to the command line data structure
                const char* name);  // the parameter to recognize. (i.e. "-n")

int get_num_param(cmd_line cptr,    // pointer to the command line data structure
                const char* name);  // the parameter to recognize. (i.e. "-n")

bool get_toggle_param(cmd_line cptr,   // pointer to the command line data structure
                const char* name);  // the parameter to recognize. (i.e. "-n")

const char* get_cmd(cmd_line cptr); // get the command string

void reset_cmd_excess(cmd_line);
char* iterate_cmd_excess(cmd_line);

// actually read the command line and fill out the data structures. do this
// after all of the parameters have been added.
void parse_cmd_line(cmd_line cmd, int argc, char** argv);

// display text that appears with the "help" parameter
void cmd_use(cmd_line cmd);

#endif
