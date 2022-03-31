/*

Принцип работы:

1) В smarr (Shared Memory Array) создается общий для всех процессов массив int*.

2) В цикле создаем n процессов для обработки файла (n = )

*/

#include "file.h"

#define MAX_SIZE (1024*16)                // Max size of buffer;
#define MAX_NUMBER_OF_ITERATIONS 1     // Every process iterate through file <= MAX_NUMBER_OF_ITERATIONS;

size_t get_size_of_file(FILE const* file) {
    size_t size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

int get_data_from_file(int** nums, FILE const* file) {

    if (file == NULL) {
        // Handle error
    }
    
    size_t fsize = get_size_of_file(file);

    int* shared_memory = (int*)shared_malloc(fsize);
    size_t* shared_memory_size = (size_t*)shared_malloc(sizeof(size_t));
    *shared_memory_size = 0;

    sem_t* semaphore = (sem_t*)shared_malloc(sizeof(sem_t));
    sem_init(&semaphore, 1, 1);

    // sem_init(semaphore);

    // chunk - number of blocks in file
    for (size_t i = 0; i <= fsize; i += MAX_SIZE * MAX_NUMBER_OF_ITERATIONS) {
        if (!fork()) {

            int* tempNums = (int*)malloc(sizeof(int));
            size_t tempNumsSize;

            tempNumsSize = inner_process_file_logic(&tempNums, file, i);
            // Handle error in process

            sem_wait(&semaphore);
            fetch(&shared_memory, shared_memory_size, tempNums, tempNumsSize);
            sem_post(&semaphore);
            
            printf("shared_memory_size = %d\n", *shared_memory_size);
            fclose(file);
            exit(EXIT_SUCCESS);
        }
    }

    while (wait(0) > 0);    //  Waiting for all child processes to finish
    // Handle errors in childs

    *nums = shared_memory;

    return *shared_memory_size;
}

int inner_process_file_logic(int** nums, FILE* file, size_t offset) {
    char* buffer = (char*)malloc(sizeof(char) * MAX_SIZE);
    char* buffer_mem = buffer;  // For free()

    if (buffer == NULL) {
        perror("get_data_from_file: buffer malloc error");
        return -1;
    }

    fseek(file, offset, SEEK_SET);
    size_t capacity = 1, size = 0;

    for (size_t i = 0; fread(buffer, sizeof(char),
        MAX_SIZE, file) != 0 && i < MAX_NUMBER_OF_ITERATIONS; i++) {

        for ( ; *buffer != '\0'; buffer++, size++) {

            if (size == capacity - 1) {
                capacity *= 2;
                *nums = realloc(*nums, sizeof(int) * capacity);

                if (*nums == NULL) {
                    free(buffer - size * sizeof(int));
                    perror("get_data_from_file: tempNums realloc error");
                    return -1;
                }
            }

            *(*nums + size) = make_number_from_chars(&buffer);
        }
    }

    free(buffer_mem);
    return size;
}

int make_number_from_chars(const char** buffer) {
    int num = 0;
    
    for( ; **buffer != '\n' && **buffer != '\0' &&
           **buffer != ' '; (*buffer)++) {

        if (**buffer == '-') {
            if (num == 0)
                num = 1;

            num *= -1;
        }

        else {
            num *= 10;
            num += **buffer - '0';
        }
    }

    return num;
}

int fetch(int** shared_memory, size_t* shared_memory_size,
          const int const* src, size_t src_size) {

    // if (dest == NULL || *dest == NULL || *src == NULL)
    //     return -1;

    for (size_t i = 0; i < src_size; ++i) {
        *(*shared_memory + *shared_memory_size + i) = *(src + i);
        // if (from != 0)
        //     printf("PID: %d; from = %d; to = %d; i = %d\n", getpid(), from, to, i);
    }

    *shared_memory_size += src_size;
    return 1;
}
