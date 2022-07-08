#include "histogram.h"
// Use shared memory

size_t interval_size(size_t size) {
    if (size > BIG)
        return size / big;

    else if (size > MEDIUM)
        return size / medium;

    else if (size < SMALL)
        return size;
        
    return size / small;
}

int hist(const int* nums, size_t* digits, const size_t from, const size_t to) {
    if (nums == NULL || digits == NULL) {
        return -1;
    }

    int temp = 0;

    for (size_t i = from; i < to; ++i) {
        temp = *(nums + i);

        // get number of digits in temp
        while (temp > 0) {
            *(digits + temp % 10) += 1;
            temp /= 10;
        }
    }

    return 1;
}

int create_hist(const int* nums, FILE* output, size_t size) {

    sem_t* semaphore = (sem_t*)shared_malloc(sizeof(sem_t));
    sem_init(semaphore, 1, 1);

    size_t* shared_digits = (size_t*)shared_malloc(sizeof(int) * NUMBER_OF_DIGITS);
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        *(shared_digits + i) = 0;
    }
    
    for (size_t i = 0; i < size; i += interval_size(size)) {
        if (!fork()) {
            // temporary array for this process
            size_t* digits = (size_t*)malloc(NUMBER_OF_DIGITS * sizeof(size_t));

            // initialize with zeros
            for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
                *(digits + i) = 0;
            }

            // get data for this process
            if (!hist(nums, digits, i, i + interval_size(size)))
                exit(EXIT_FAILURE);

            sem_wait(semaphore);
            merge(shared_digits, digits);
            sem_post(semaphore);

            free(digits);
            exit(EXIT_SUCCESS);
        }
    }

    while (wait(0) > 0); // Waiting for all child processes

    /* ---- print in output (can be stdin) ---- */

    fprintf(output, "%s\n", "--- HISTOGRAM ---");

    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        fprintf(output, "[%d] %ld\n", i, *(shared_digits + i));
    }

    int med = median(shared_digits);
    fprintf(output, "Median: %d\n", med);

    shared_free(semaphore, sizeof(sem_t));
    shared_free(shared_digits, sizeof(int) * NUMBER_OF_DIGITS);

    return med;
}

int merge(size_t* shared_digits, size_t* src) {
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        *(shared_digits + i) += *(src + i);
    }

    return 1;
}

int median(size_t* shared_digits) {
    
    size_t sum = 0;
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        sum += shared_digits[i];
    }

    size_t temp = 0;
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        temp += shared_digits[i];

        if (temp > sum / 2) {
            return i;
        }
    }

    return NUMBER_OF_DIGITS - 1;
}