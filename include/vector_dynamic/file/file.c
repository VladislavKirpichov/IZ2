/*

Принцип работы:

1) В smarr (Shared Memory Array) создается общий для всех процессов массив int*.

2) В цикле создаем n процессов для обработки файла (n = )

*/

#include "file.h"

#define BUFFER_SIZE (1024*16)                // Max size of buffer;
#define MAX_NUMBER_OF_ITERATIONS 4     // Every process iterate through file <= MAX_NUMBER_OF_ITERATIONS;

size_t get_size_of_file(FILE* file) {
    size_t size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

int get_data_from_file(int** nums, FILE* file) {

    if (file == NULL) {
        perror("file if empty!");
        return -1;
    }
    
    size_t fsize = get_size_of_file(file);

    int* shared_memory = (int*)shared_malloc(fsize);
    size_t* shared_memory_size = (size_t*)shared_malloc(sizeof(size_t));
    *shared_memory_size = 0;

    sem_t* semaphore = (sem_t*)shared_malloc(sizeof(sem_t));
    sem_init(semaphore, 1, 1);

    int status = 0;

    // chunk - number of blocks in file
    for (size_t i = 0; i < fsize; i += BUFFER_SIZE * MAX_NUMBER_OF_ITERATIONS) {
        if (!fork()) {

            int* tempNums = (int*)malloc(sizeof(int));
            size_t tempNumsSize;

            sem_wait(semaphore);
            tempNumsSize = inner_process_file_logic(&tempNums, file, i);
            sem_post(semaphore);
            // printf("tempNumsSize = %d\n", tempNumsSize);
            
            if (tempNums < 0) {
                perror("inner_process_file_logic error");
                fclose(file);
                free(tempNums);
                exit(EXIT_FAILURE);
            }

            sem_wait(semaphore);
            *shared_memory_size = fetch(&shared_memory, *shared_memory_size, tempNums, tempNumsSize);
            sem_post(semaphore);

            free(tempNums);
            fclose(file);
            exit(EXIT_SUCCESS);
        }
    }

    //  Waiting for all child processes to finish
    while (wait(&status) > 0) {
        if (&status < 0) 
            return -1;
    };
    // Handle errors in childs
    printf("shared_memory_size = %d\n", *shared_memory_size);

    *nums = shared_memory;

    return *shared_memory_size;
}

int inner_process_file_logic(int** nums, FILE* file, size_t offset) {
    char* buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    char* buffer_mem = buffer;  // For free()

    if (buffer == NULL) {
        perror("get_data_from_file: buffer malloc error");
        return -1;
    }

    fseek(file, offset, SEEK_SET);
    size_t capacity = 1, size = 0;
    ssize_t number_of_bytes_read = 0;

    for (size_t i = 0; (number_of_bytes_read = fread(buffer, sizeof(char),
        BUFFER_SIZE, file)) > 0 && i < MAX_NUMBER_OF_ITERATIONS; i++) {
        *(buffer + number_of_bytes_read) = '\0';

        for ( ; *buffer != '\0'; buffer++, size++) {
            if (size == capacity - 1) {
                capacity *= 2;
                *nums = realloc(*nums, sizeof(int) * capacity);

                if (*nums == NULL) {
                    free(buffer - BUFFER_SIZE * i);
                    perror("get_data_from_file: tempNums realloc error");
                    return -1;
                }
            }

            *(*nums + size) = make_number_from_chars(&buffer);
        }

        buffer = buffer_mem;
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

int fetch(int** shared_memory, size_t shared_memory_size,
          const int const* src, size_t src_size) {

    if (shared_memory == NULL || *shared_memory == NULL || src == NULL)
        return -1;

    for (size_t i = 0; i < src_size; ++i) {
        *(*shared_memory + shared_memory_size + i) = *(src + i);
    }

    return shared_memory_size + src_size;
}
