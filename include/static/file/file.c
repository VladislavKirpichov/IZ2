#include "file.h"

#define MAX_SIZE 1024*16

int* get_data_from_file(FILE* file) {

    if (file == NULL) {
        perror("Error opening file!");
    }

    char* buffer = (char*)malloc(sizeof(char) * MAX_SIZE);

    if (buffer == NULL) {
        perror("malloc error");
        return NULL;
    }

    int* nums = (int*)malloc(sizeof(int));

    if (nums == NULL) {
        free(buffer);
        perror("malloc error");
        return NULL;
    }

    size_t capacity = 1, size = 0;

    while (fread(buffer, sizeof(char), MAX_SIZE - 1, file)) {
        buffer[MAX_SIZE - 1] = '\0';
        for (; size < MAX_SIZE / sizeof(int); size++) {

            if (size == capacity - 1) {
                capacity *= 2;
                nums = realloc(nums, sizeof(int) * capacity);

                if (nums == NULL) {
                    free(buffer - size * sizeof(int));
                    return NULL;
                }
            }

            char** temp = &buffer;
            nums[size] = make_number_from_chars(&buffer);

            buffer++;
        }
    }

    free(buffer - MAX_SIZE);
    return nums;
}

// int make_vector(char* buffer, vector* nums) {
//     int temp;
//
//     for (size_t i = 0; i < MAX_SIZE; ++i) {
//         temp = make_number_from_chars(buffer);
//     }
// }

int make_number_from_chars(const char** buffer) {
    int num = 0;
    
    while(**buffer != '\n' || **buffer != '\0') {
        if (**buffer == '-') {
            if (num == 0)
                num = 1;

            num *= -1;
        }

        else {
            num *= 10;
            num += **buffer - '0';
        }

        *buffer++;
    }
    return num;
}