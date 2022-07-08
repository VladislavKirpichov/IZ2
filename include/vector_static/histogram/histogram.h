/*
  This file describes methods for constructing a histogram
*/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_DIGITS 10

int hist(const int* nums, size_t* digits, size_t size);
int create_hist(const int* nums, FILE* output, size_t size);
int median(size_t* digits);

#endif
