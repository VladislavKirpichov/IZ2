//
// Created by vladislav on 26.03.2022.
//

#include "vector.h"
#include "file.h"
#include "histogram.h"

vector* vector__init() {
    vector* nums = (vector*)malloc(sizeof(vector));
    nums->arr = NULL;
    nums->size = 0;

    return nums;
}

vector* vector__init_with_file(FILE* file) {
    vector* nums = (vector*)malloc(sizeof(vector));
    nums->arr = (int*)malloc(sizeof(int));
    nums->size = get_data_from_file(&(nums->arr), file);

    if (nums->size == -1) {
        free(nums);
        return NULL;
    }

    return nums;
}

int vector__free(vector* vctr) {
    free(vctr->arr);
    free(vctr);
}