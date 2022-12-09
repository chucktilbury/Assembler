#ifndef _COMMON_HASH_H
#define _COMMON_HASH_H

#include <stdlib.h>

typedef enum {
    HASH_OK,
    HASH_DUP,
    HASH_NF,
} HashResult;

typedef struct {
    const char* key;
    void* data;
    size_t size;
} HashNode;

/*
 * If a node's key is NULL, but the bucket pointer in the table
 * is not NULL, then the bucket is a tombstone.
 */
typedef struct {
    HashNode** table;
    int cap;
    int count;
    int tombstones;
} Hash;

Hash* HashCreate();
void HashDestroy(Hash* table);
HashResult HashInsert(Hash* table, const char* key, void* data, size_t size);
HashResult HashFind(Hash* tab, const char* key, void* data, size_t size);
HashResult HashRemove(Hash* tab, const char* key);
void HashDump(Hash* tab);

#endif /* _COMMON_HASH_H */
