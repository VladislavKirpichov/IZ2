//
// Created by vladislav on 26.03.2022.
//

#ifndef IZ2_NUMS_VECTOR_H
#define IZ2_NUMS_VECTOR_H

#include <stdio.h>
#include <stdlib.h>

struct vector {
    int* arr;
    
    size_t size;
    size_t capacity;
};
typedef struct vector vector;

vector* vector__init();
vector* vector__init_with_file(FILE* file);

int distribution(vector nums);
int compare(vector nums);

#endif //IZ2_NUMS_VECTOR_H
