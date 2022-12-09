#ifndef _ASSEMBLER_ASM_ERRORS_H
#define _ASSEMBLER_ASM_ERRORS_H

void syntaxError(const char* fmt, ...);
void fatalError(const char* fmt, ...);
int getErrors();
int getWarnings();

#endif /* _ASSEMBLER_ASM_ERRORS_H */
