#include "file.h"

#define MAX_SIZE 1024*16

int get_data_from_file(int** nums, FILE* file) {

    if (file == NULL) {
        perror("Error opening file!");
    }

    if (nums == NULL) {
        perror("Nums is empty!");
    }

    char* buffer = (char*)malloc(sizeof(char) * MAX_SIZE);
    char* buffer_mem = buffer;  // For free()

    if (buffer == NULL) {
        perror("get_data_from_file: buffer malloc error");
        return -1;
    }
    size_t capacity = 1, size = 0;

    while (fread(buffer, sizeof(char), MAX_SIZE, file)) {
        for (; *buffer != '\0'; buffer++, size++) {

            if (size == capacity - 1) {
                capacity *= 2;
                *nums = realloc(*nums, sizeof(int) * capacity);

                if (*nums == NULL) {
                    free(buffer - size * sizeof(int));
                    perror("get_data_from_file: nums realloc error");
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
    
    for(; **buffer != '\n' && **buffer != '\0'; (*buffer)++) {
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