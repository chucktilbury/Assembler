#ifndef ASMERRORS_H
#define ASMERRORS_H

void syntaxError(const char* fmt, ...);
void fatalError(const char* fmt, ...);
int getErrors();
int getWarnings();

#endif