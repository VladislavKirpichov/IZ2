#include "shared_memory.h"

void* shared_malloc(size_t size) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED | MAP_ANONYMOUS;

    void* mem = mmap(NULL, size, prot, flags, -1, 0);
    return mem;
}

void shared_free(void* ptr, size_t size) {
    munmap(ptr, size);
}

// void* shared_realloc(void* ptr, size_t prev_size, size_t size) {
//     int prot = PROT_READ | PROT_WRITE;
//     int flags = MAP_SHARED | MAP_ANONYMOUS;

//     for (size_t i = 0; i < )
// }