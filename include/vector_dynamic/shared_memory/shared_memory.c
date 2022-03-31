#include "shared_memory.h"

void* shared_malloc(size_t size) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED | MAP_ANONYMOUS;

    void* mem = mmap(NULL, size, prot, flags, -1, 0);
    return mem;
}

void* shared_realloc(size_t size);