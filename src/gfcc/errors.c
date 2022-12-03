
#include "goldfish.h"
#include "scanner.h"
#include <stdarg.h>

static int errors = 0;
static int warnings = 0;
static FILE* err_stream = NULL;

void error(const char* fmt, ...) {

    if(err_stream == NULL)
        err_stream = stderr;

    fprintf(err_stream, "%s:%d: syntax error: ", get_file_name(), get_line_no());

    va_list args;
    va_start(args, fmt);
    vfprintf(err_stream, fmt, args);
    va_end(args);

    fprintf(err_stream, "\n");
    errors++;
}

void warning(const char* fmt, ...) {

    if(err_stream == NULL)
        err_stream = stderr;

    fprintf(err_stream, "%s:%d: warning: ", get_file_name(), get_line_no());

    va_list args;
    va_start(args, fmt);
    vfprintf(err_stream, fmt, args);
    va_end(args);

    fprintf(err_stream, "\n");
    warnings++;
}

void fatal(const char* fmt, ...) {

    if(err_stream == NULL)
        err_stream = stderr;

    fprintf(err_stream, "fatal error: ");

    va_list args;
    va_start(args, fmt);
    vfprintf(err_stream, fmt, args);
    va_end(args);

    fprintf(err_stream, "\n");
    exit(1);
}

int get_errors() {
    return errors;
}

int get_warnings() {
    return warnings;
}

void set_err_stream(FILE* s) {
    err_stream = s;
}