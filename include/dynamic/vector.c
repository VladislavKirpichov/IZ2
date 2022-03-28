//
// Created by vladislav on 26.03.2022.
//

#include "vector.h"

#define BUFFER_SIZE (1024*16)   // 16kb

vector* vector__init() {
    vector* nums = malloc(sizeof(vector));
    nums->arr = NULL;
    nums->size = 0;
}

int vector__new(vector* nums, FILE* file) {
    nums->size = 0;
}