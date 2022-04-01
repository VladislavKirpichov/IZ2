#include <stdlib.h>
#include <sys/mman.h>

void* shared_malloc(size_t size);
void shared_free(void* ptr, size_t size);
// void* shared_realloc(void* ptr, size_t prev_size, size_t size);
