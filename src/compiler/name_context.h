#ifndef _NAME_CONTEXT_H_
#define _NAME_CONTEXT_H_

void initNameContext();
int pushNameContext(const char* name);
const char* popNameContext();
const char* getNameContext();

#endif