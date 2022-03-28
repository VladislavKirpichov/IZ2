//
// Created by vladislav on 26.03.2022.
//

#include "vector.h"
#include "file.h"

#define BUFFER_SIZE (1024*16)   // 16kb

vector* vector__init() {
    vector* nums = malloc(sizeof(vector));
    nums->arr = NULL;
    nums->size = 0;

    return nums;
}

vector* vector__init_with_file(FILE* file) {
    vector* nums = malloc(sizeof(vector));
    
    nums->arr = get_data_from_file(file);

    return nums;
}