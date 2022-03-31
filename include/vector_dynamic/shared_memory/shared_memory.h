#include <stdlib.h>
#include <sys/mman.h>

void* shared_malloc(size_t size);
void* shared_realloc(size_t size);
