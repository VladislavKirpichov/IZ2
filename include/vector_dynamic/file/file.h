/*

// This library describes how to work with files using multiple processes

WORK METHOD:

    ------------------------------- Parent Process(file) -------------------------------
    |                                       |                                          |
    Child Process(chunk)    ...     Child Process(chunk)    ...     Child Process(chunk)
    |                                       |                                          |
    |                                       |                                          |
    inner_process_logic             inner_process_logic              inner_process_logic
    |                                       |                                          |
    |                                       |                                          |
    ------------------------------->  combine_data()  <---------------------------------
                                            |
                                            |
                                       returns nums


1)  The file is split into chunks.

2)  The parent process waits until the child processes created by it process all the chunks
    Each child process writes the data received from the file to a temporary array, which
    available only to this process

3)  The child process combines the received data with what is already stored in a common array 

---

RETURN STATUS:

success:                0         
error in memory:        -1
error in input data:    1


*/

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include "shared_memory.h"

#define BUFFER_SIZE (1024*16)
#define MAX_CHUNK_FILE_NAME_SIZE 16

// file sizes
#define KB 1024
#define MB (1024 * 1024)
#define GB (1024 * 1024 * 1024)

enum RETURN_STATUSES {
    SUCCESS = 0,
    SYS_MEMORY_ERROR = -1,  // malloc, realloc errors
    INPUT_DATA_ERROR = 1,
    PROCESS_ERROR = -2,
};

enum NUMBER_OF_ITERATIONS {
    KB_128 = 4,
    MB_1 = 8,
    MB_128 = 16,
    MB_512 = 64,
    GB_1 = 128
};

size_t size_of_file(FILE* file); // returns number of bytes in file
size_t chunk_size(size_t size);

int get_data_from_file(int** nums, FILE* file);
int make_number_from_chars(char** buffer);
int create_chunk(FILE* src_file, FILE** chunk, int size_of_chunk, int chunk_index);

int inner_process_file_logic(int** nums, FILE* chunk);
int combine_data(int** shared_memory, size_t shared_memory_size,
          const int* src, size_t src_size);

#endif
