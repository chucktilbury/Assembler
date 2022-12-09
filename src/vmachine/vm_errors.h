#ifndef _VMACHINE_VM_ERRORS_H
#define _VMACHINE_VM_ERRORS_H

void runtimeError(const char* fmt, ...);
void fatalError(const char* fmt, ...);
int getErrors();

#endif /* _VMACHINE_VM_ERRORS_H */
