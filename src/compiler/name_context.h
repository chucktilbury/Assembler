#ifndef _COMPILER_NAME_CONTEXT_H
#define _COMPILER_NAME_CONTEXT_H

void initNameContext();
int pushNameContext(const char* name);
int popNameContext();
const char* getNameContext();
const char* createName(const char* name);

#endif /* _COMPILER_NAME_CONTEXT_H */
