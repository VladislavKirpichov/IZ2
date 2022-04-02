/*

// This library creates histograms of the distribution of digital input data.

WORK METHOD:
1)  The enum INTERVAL_SIZE defines the intervals at which the processes will work.
    Number of processes = size / INTERVAL_SIZE (where size is the number of inputs)

2)  Each process has its own counter of digits and works on a certain interval
    data (from .. to). As a result, each process has a ready set of data
    about the number of all digits in its interval

3)  In the merge function, the data received in each process is combined
    with the main array, which stores the total number of digits in the entire input array nums
    
*/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include "shared_memory.h"

#define NUMBER_OF_DIGITS 10

// file sizes
#define SMALL 1024          // 1kb
#define MEDIUM (1024 * 16)  // 16kb
#define BIG (1024 * 128)    // 128kb

enum INTERVAL_SIZE {
    small = 4,
    medium = 32,
    big = 128,
};

int hist(const int* nums, size_t* digits, const size_t from, const size_t to);
int create_hist(const int* nums, FILE* output, size_t size);
int merge(size_t* shared_digits, size_t* src);
int median(size_t* shared_digits);

#endif
