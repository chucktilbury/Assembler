#include "common.h"
#include "name_context.h"
#include "list.h"

List* context = NULL;

// List* listCreate();
// void listDestroy(List* lst);
// ListErr listAdd(List* lst, void* val, size_t size);
// ListErr listGet(List* lst, int idx, void* val, size_t size);
// ListErr listSet(List* lst, int idx, void* val, size_t size);
// ListErr listPush(List* lst, void* val, size_t size);
// ListErr listPop(List* lst, void* val, size_t size);
// ListErr listPeek(List* lst, void* val, size_t size);
// int listGetSize(List* lst);
// void* listGetRaw(List* lst);

void initNameContext() {

    context = listCreate();
}

int pushNameContext(const char* name) {
printf("push name: %s\n", name);
    return listPush(context, (void*)name, sizeof(const char*));
}

//const char* popNameContext() {
int popNameContext() {

    //const char* name;

    // if(LIST_OK == listPop(context)) //, (void*)name, sizeof(const char*)))
    //     return name;
    // else
    //     return NULL;
    return listPop(context);
}

const char* getNameContext() {

    ListItem** lst = listGetRaw(context);
    int len = listGetSize(context);
    char* result = _alloc(1);
    result[0] = '\0';

    for(int i = 0; i < len; i++) {
        const char* val = lst[i]->data;
        result = _realloc(result, strlen(result)+strlen(val)+3);
        strcat(result, val);
        if(i+1 < len)
            strcat(result, ".");
    }

    return result;
}

const char* createName(const char* name) {

    char* ctxt = (char*)getNameContext();
    ctxt = _realloc((void*)ctxt, strlen(ctxt)+strlen(name)+3);
    strcat(ctxt, ".");
    strcat(ctxt, name);

    return ctxt;
}
