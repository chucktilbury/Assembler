
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "memory.h"

#define MIN(l, r) (((l) < (r)) ? (l) : (r))

List* listCreate() {

    List* val = _alloc_ds(List);
    val->cap = 1;
    val->len = 0;
    val->index = 0;
    val->list = _alloc_ds_array(ListItem*, val->cap);

    return val;
}

void listDestroy(List* lst) {

    if(lst != NULL) {
        for(int i = 0; i < lst->len; i++) {
            _free(lst->list[i]->data);
            _free(lst->list[i]);
        }
        _free(lst->list);
        _free(lst);
    }
}

ListErr listAdd(List* lst, void* val, size_t size) {

    if(lst->len + 1 > lst->cap) {
        lst->cap <<= 1;
        lst->list = _realloc_ds_array(lst->list, ListItem*, lst->cap);
    }

    ListItem* item = _alloc_ds(ListItem);
    item->data = _alloc(size);
    memcpy(item->data, val, size);
    item->size = size;

    lst->list[lst->len] = item;
    lst->len++;

    return LIST_OK;
}

ListErr listGet(List* lst, int idx, void* val, size_t size) {

    if(idx < 0 || idx >= lst->len)
        return LIST_BOUNDS;

    ListItem* item = lst->list[idx];
    ListErr err = LIST_OK;
    if(item->size != size)
        err = LIST_SIZE;

    memcpy(val, item->data, MIN(item->size, size));

    return err;
}

ListErr listSet(List* lst, int idx, void* val, size_t size) {

    if(idx < 0 || idx >= lst->len)
        return LIST_BOUNDS;

    ListItem* item = lst->list[idx];
    ListErr err = LIST_OK;
    if(item->size != size)
        err = LIST_SIZE;

    memcpy(item->data, val, MIN(size, item->size));

    return err;
}

ListErr listPush(List* lst, void* val, size_t size) {

    return listAdd(lst, val, size);
}

ListErr listPop(List* lst, void* val, size_t size) {

    if(lst->len > 0) {
        ListItem* item = lst->list[--lst->len];
        ListErr err = LIST_OK;
        if(item->size != size)
            err = LIST_SIZE;

        memcpy(item->data, val, MIN(size, item->size));

        _free(lst->list[lst->len]->data);
        _free(lst->list[lst->len]);

        return err;
    }
    else
        return LIST_BOUNDS;
}

ListErr listPeek(List* lst, void* val, size_t size) {

    if(lst->len > 0) {
        ListItem* item = lst->list[lst->len - 1];
        ListErr err = LIST_OK;
        if(item->size != size)
            err = LIST_SIZE;

        memcpy(item->data, val, MIN(size, item->size));

        return err;
    }
    else
        return LIST_BOUNDS;
}

int listGetSize(List* lst) {

    return lst->len;
}

void* listGetRaw(List* lst) {

    return lst->list;
}

#ifdef TEST
/*
 * Build string:
 * clang -Wall -Wextra -g -o t list_examp.c memory.c -DTEST
 */

void dump(List* arr) {

    printf("cap: %d\n", arr->cap);
    printf("len: %d\n", arr->len);
    for(int i = 0; i < arr->len; i++)
        printf("%3d. %s\n", i + 1, (char*)arr->list[i]);

    printf("\n");
}

int main() {

    const char* strs[] = { "string one",
                           "string two",
                           "string three",
                           "string four",
                           "string five",
                           "string six",
                           "string seven",
                           "string eight",
                           "string nine",
                           "string ten",
                           NULL };

    List* arr = ListCreate(sizeof(char*));

    for(int i = 0; strs[i] != NULL; i++)
        ListAdd(arr, (void*)strs[i]);
    dump(arr);

    ListDestroy(arr);
    return 0;
}

#endif
