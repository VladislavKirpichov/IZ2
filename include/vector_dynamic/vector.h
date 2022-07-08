//
// Created by vladislav on 26.03.2022.
//

#ifndef IZ2_NUMS_VECTOR_H
#define IZ2_NUMS_VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "file.h"
#include "histogram.h"
#include "shared_memory.h"

struct vector {
    int* arr;
    size_t size;
};
typedef struct vector vector;

vector* vector__init();
vector* vector__init_with_file(FILE* file);
int vector__free(vector* vctr);

#endif //IZ2_NUMS_VECTOR_H
