#include "gas.h"
#include <stdarg.h>

static int errors = 0;
static int warnings = 0;

void syntaxError(const char* fmt, ...)
{
    va_list args;
    fprintf(stderr, "syntax error: %d: ", get_line_no());

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    errors++;
}

void syntaxWarning(const char* fmt, ...)
{
    va_list args;
    fprintf(stderr, "warning: ");

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    warnings++;
}

void fatalError(const char* fmt, ...)
{
    va_list args;

    fprintf(stderr, "error: ");

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    errors++;
    exit(1);
}

int getErrors()
{
    return errors;
}

int getWarnings()
{
    return warnings;
}

