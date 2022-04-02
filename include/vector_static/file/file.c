#include "file.h"

#define BUFFER_SIZE 1024*16

int get_data_from_file(int** nums, FILE* file) {

    if (file == NULL) {
        perror("Error opening file!");
    }

    if (nums == NULL) {
        perror("Nums is empty!");
    }

    char* buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);
    char* buffer_mem = buffer;  // For free()

    if (buffer == NULL) {
        perror("get_data_from_file: buffer malloc error");
        return -1;
    }

    int* tempNums = (int*)malloc(sizeof(int));
    if (tempNums == NULL) {
        perror("get_data_from_file: tempNums malloc error");
        return -1;
    }
    
    size_t capacity = 1, size = 0;
    ssize_t number_of_bytes_read = 0;

    while ((number_of_bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, file)) > 0) {
        *(buffer + number_of_bytes_read) = '\0';
        
        for ( ; *buffer != '\0'; buffer++, size++) {

            if (size == capacity - 1) {
                capacity *= 2;
                tempNums = realloc(tempNums, sizeof(int) * capacity);

                if (tempNums == NULL) {
                    free(buffer - size * sizeof(int));
                    perror("get_data_from_file: nums realloc error");
                    return -1;
                }
            }

            *(tempNums + size) = make_number_from_chars(&buffer);
        }

        buffer = buffer_mem;
    }

    *nums = tempNums;
    
    free(buffer_mem);
    fseek(file, 0, SEEK_SET);
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