
#include "vmachine.h"
#include <stdarg.h>

static int errors = 0;

void runtimeError(const char* fmt, ...)
{
    va_list args;
    fprintf(stderr, "\nruntime error: %d: ", getIndex());

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    errors++;
}

void fatalError(const char* fmt, ...)
{
    va_list args;

    fprintf(stderr, "\ninternal error: %d: ", getIndex());

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

