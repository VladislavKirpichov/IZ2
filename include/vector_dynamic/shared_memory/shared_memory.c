#include "shared_memory.h"

void* shared_malloc(size_t size) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED | MAP_ANONYMOUS;

    void* mem = mmap(NULL, size, prot, flags, -1, 0);
    return mem;
}

void shared_free(void* ptr, size_t size) {
    if (ptr == NULL) {
        fprintf(stderr, "shared_free(). ptr is empty. pid = %d", getpid());
    }
    munmap(ptr, size);
}

void* shared_realloc(void* ptr, size_t prev_size, size_t new_size) {
    if (ptr == NULL) {
        fprintf(stderr, "shared_realloc(). ptr is empty. pid = %d", getpid());
        return NULL;
    }

    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED | MAP_ANONYMOUS;

    void* new_mem = shared_malloc(new_size);

    for (size_t i = 0; i < prev_size; ++i) {
        *(char*)(new_mem + i) = *(char*)(ptr + i);
    }

    shared_free(ptr, new_size);
    return new_mem;
}