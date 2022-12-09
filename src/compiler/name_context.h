#ifndef _NAME_CONTEXT_H_
#define _NAME_CONTEXT_H_

void initNameContext();
int pushNameContext(const char* name);
int popNameContext();
const char* getNameContext();
const char* createName(const char* name);

#endif