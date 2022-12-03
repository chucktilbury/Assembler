#ifndef _ERRORS_H_
#define _ERRORS_H_

void error(const char* fmt, ...);
void warning(const char* fmt, ...);
void fatal(const char* fmt, ...);

int get_errors();
int get_warnings();

void set_err_stream(FILE* s);

#endif
