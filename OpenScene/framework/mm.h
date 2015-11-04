#ifndef MM_H
#define MM_H

#include <stdlib.h>

void* cg_malloc(size_t size);

void* cg_realloc(void* ptr, size_t size);

void cg_free(void* ptr);

void cg_memcheck(void);

#endif //MM_H

