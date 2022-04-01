#include "file.h"

size_t get_size_of_file(FILE* file) {
    size_t size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

size_t chunk_size(size_t size) {
    if (size > GB)
        return BUFFER_SIZE * GB_1;

    else if (size > 512*MB)
        return BUFFER_SIZE * MB_512;

    else if (size > 128*MB)
        return BUFFER_SIZE * MB_128;
    
    else if (size > MB)
        return BUFFER_SIZE * MB_1;

    return BUFFER_SIZE * KB_128;
}

int create_chunk(FILE* src_file, FILE** chunk, int size_of_chunk, int chunk_index) {
    // generate name of chunk file
    char filename[MAX_CHUNK_FILE_NAME_SIZE];
    sprintf(filename, "chunk_%d.txt", chunk_index);

    *chunk = fopen(filename, "w+");

    char buffer[size_of_chunk];
    size_t number_of_bytes_read = 0;

    // read from src
    number_of_bytes_read = fread(buffer, sizeof(char), size_of_chunk, src_file);
    buffer[number_of_bytes_read] = '\0';
    // printf("number_of_bytes_read = %d; size_of_chunk = %d\n", number_of_bytes_read, size_of_chunk);

    fwrite(buffer, sizeof(char), number_of_bytes_read, *chunk);
    fseek(*chunk, 0, SEEK_SET);
    return 1;
}

int get_data_from_file(int** nums, FILE* file) {

    if (file == NULL) {
        perror("file if empty!");
        return -1;
    }
    
    size_t fsize = get_size_of_file(file);

    // create array in shared memory (with mmap)
    int* shared_memory = (int*)shared_malloc(fsize);
    size_t* shared_memory_size = (size_t*)shared_malloc(sizeof(size_t));
    *shared_memory_size = 0;

    // binary semaphore (mutex). Created in shared memory
    sem_t* semaphore = (sem_t*)shared_malloc(sizeof(sem_t));
    sem_init(semaphore, 1, 1);

    FILE* chunk;
    size_t chunkSize = chunk_size(fsize);
    int status = 0;

    // chunk - number of blocks in file
    for (size_t i = 0; i < fsize; i += chunkSize) {

        create_chunk(file, &chunk, chunkSize, i / chunkSize);

        if (!fork()) {
            // temporary array for this process
            int* tempNums = (int*)malloc(sizeof(int));
            size_t tempNumsSize = 0;

            tempNumsSize = inner_process_file_logic(&tempNums, chunk);
            
            if (tempNumsSize < 0) {
                perror("inner_process_file_logic error");
                fclose(file);
                free(tempNums);
                exit(EXIT_FAILURE);
            }

            sem_wait(semaphore);
            *shared_memory_size = combine_data(&shared_memory, *shared_memory_size, tempNums, tempNumsSize);
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

    *nums = shared_memory;

    return *shared_memory_size;
}

int inner_process_file_logic(int** nums, FILE* chunk) {
    char* buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    char* buffer_mem = buffer;  // For free()

    if (buffer == NULL) {
        perror("get_data_from_file: buffer malloc error");
        return -1;
    }

    size_t capacity = 1, size = 0;
    ssize_t number_of_bytes_read = 0;

    for (size_t i = 0; (number_of_bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, chunk)) > 0; i++) {
        *(buffer + number_of_bytes_read) = '\0';

        for ( ; *buffer != '\0'; buffer++, size++) {
            // printf("%c", *buffer);
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

int make_number_from_chars(char** buffer) {
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

int combine_data(int** shared_memory, size_t shared_memory_size,
          const int const* src, size_t src_size) {

    if (shared_memory == NULL || *shared_memory == NULL || src == NULL)
        return -1;

    for (size_t i = 0; i < src_size; ++i) {
        *(*shared_memory + shared_memory_size + i) = *(src + i);
    }

    return shared_memory_size + src_size;
}
