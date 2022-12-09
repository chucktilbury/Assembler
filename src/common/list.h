
#ifndef _COMMON_LIST_H
#define _COMMON_LIST_H

#include <stdlib.h>

typedef enum {
    LIST_OK,
    LIST_END,
    LIST_BOUNDS,
    LIST_ERROR,
    LIST_SIZE,
} ListErr;

typedef struct {
    void* data;
    size_t size;
} ListItem;

typedef struct {
    ListItem** list;
    int cap;
    int len;
    int index;
} List;

List* listCreate();
void listDestroy(List* lst);
ListErr listAdd(List* lst, void* val, size_t size);
ListErr listGet(List* lst, int idx, void* val, size_t size);
ListErr listSet(List* lst, int idx, void* val, size_t size);
ListErr listPush(List* lst, void* val, size_t size);
ListErr listPop(List* lst); //, void* val, size_t size);
ListErr listPeek(List* lst, void* val, size_t size);
int listGetSize(List* lst);
ListItem** listGetRaw(List* lst);


#endif /* _COMMON_LIST_H */
