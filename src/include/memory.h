#ifndef MEMORY_H
#define MEMORY_H

#if 0

//#ifdef MEMORY_DEBUG
#define _init_memory()
#define _uninit_memory()
#define _alloc(s) malloc(s)
#define _alloc_ds(t) (t*)malloc(sizeof(t))
#define _alloc_ds_array(t, n) (t*)malloc(sizeof(t) * (n))
#define _realloc(p, s) realloc((p), (s))
#define _realloc_ds_array(p, t, n) (t*)realloc((p), sizeof(t) * (n))
#define _copy_str(s) strdup(s)

#else

#include "gc.h"
//#include "cord.h"

#define _init_memory() GC_init()
#define _uninit_memory() GC_deinit()
#define _alloc(s) GC_malloc(s)
#define _alloc_ds(t) (t*)GC_malloc(sizeof(t))
#define _alloc_ds_array(t, n) (t*)GC_malloc(sizeof(t) * (n))
#define _realloc(p, s) GC_realloc((p), (s))
#define _realloc_ds_array(p, t, n) (t*)GC_realloc((p), sizeof(t) * (n))
#define _copy_str(s) GC_strdup(s)
#define _free(p)
#endif

#endif
