//
// Created by vladislav on 26.03.2022.
//

#ifndef IZ2_NUMS_VECTOR_H
#define IZ2_NUMS_VECTOR_H

#include <stdio.h>
#include <stdlib.h>

struct vector {
    int* arr;
    int size;
};
typedef struct vector vector;

int vector__init(vector* nums);
int vector__new(vector* nums, FILE* file);

int distribution(vector nums);
int compare(vector nums);

#endif //IZ2_NUMS_VECTOR_H
