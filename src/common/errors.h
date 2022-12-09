#ifndef _COMMON_ERRORS_H
#define _COMMON_ERRORS_H

void runtimeError(const char* fmt, ...);
void syntaxError(const char* fmt, ...);
void syntaxWarning(const char* fmt, ...);
void fatalError(const char* fmt, ...);
int getErrors();
int getWarnings();

#endif /* _COMMON_ERRORS_H */
