#ifndef VM_ERRORS_H
#define VM_ERRORS_H

void runtimeError(const char* fmt, ...);
void fatalError(const char* fmt, ...);
int getErrors();

#endif
