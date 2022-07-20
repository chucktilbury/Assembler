#ifndef ERRORS_H
#define ERRORS_H

void runtimeError(const char* fmt, ...);
void syntaxError(const char* fmt, ...);
void syntaxWarning(const char* fmt, ...);
void fatalError(const char* fmt, ...);
int getErrors();
int getWarnings();

#endif