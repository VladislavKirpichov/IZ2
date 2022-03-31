//  This file describes methods for constructing a histogram

/*  Принцип работы hist:

**  1) Создается n процессов. n = log(arr.size) при add.size >= 2^10

**  2) Для каждого процесса также создается массив digits[NUMBER_OF_DIGITS]
**     в который данный процесс будет записывать считанные данные

**  3) После завершения работы всех процессов массивы digits из них объединяются в один
**     (достаточно просто проссумировать значения в одинаковых индексах массивов)
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "shared_memory.h"

#define NUMBER_OF_DIGITS 10

int* hist(const int const* nums, size_t* digits, const size_t from, const size_t to);
int create_hist(const int const* nums, size_t size);
int merge(size_t* shared_digits, size_t* src);
int median(const int const* nums, size_t size);