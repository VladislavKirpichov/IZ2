//
// Created by vladislav on 26.03.2022.
//

#include "vector.h"

#define BUFFER_SIZE (1024*16)   // 16kb

int vector__init(vector* nums) {
    nums->arr = NULL;
    nums->size = 0;
}

int vector__new(vector* nums, FILE* file) {
    nums->size = 0;

    char buffer[BUFFER_SIZE];
}