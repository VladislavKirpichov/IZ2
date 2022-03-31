#include "histogram.h"
// Use shared memory

int* hist(const int const* nums, size_t* digits, const size_t from, const size_t to) {
    if (nums == NULL) {
        perror("NULL nums");
        return -1;
    }

    if (to == 0) {
        perror("nums is empty!");
        return 0;
    }
    int temp = 0;

    for (size_t i = from; i < to; ++i) {
        temp = *(nums + i);

        while (temp > 0) {
            *(digits + temp % 10) += 1;
            temp /= 10;
        }
    }

    return 1;
}

int create_hist(const int const* nums, size_t size) {

    size_t n = 2000;   // n = log(arr.size) - кол-во процессов

    sem_t* semaphore = (sem_t*)shared_malloc(sizeof(sem_t));
    sem_init(semaphore, 1, 1);
    
    int fd = open("answer.txt", 'r');
    int* shared_digits = (int*)shared_malloc(sizeof(int) * NUMBER_OF_DIGITS);
    
    for (size_t i = 0; i < size; i += size / 2 + 1) {
        if (!fork()) {
            size_t* digits = (size_t*)malloc(NUMBER_OF_DIGITS * sizeof(size_t));
            for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
                *(digits + i) = 0;
            }

            if (!hist(nums, digits, i, i + size / 2))
                exit(EXIT_FAILURE);

            sem_wait(semaphore);
            merge(shared_digits, digits);
            sem_post(semaphore);

            exit(EXIT_SUCCESS);
        }
    }

    while (wait(0) > 0); // Waiting for all child processes

    printf("%s\n", "--- HISTOGRAM ---");

    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        printf("[%d] %d\n", i, *(shared_digits + i));
    }
}

// SEGSEGV
int merge(size_t* shared_digits, size_t* src) {
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        *(shared_digits + i) += *(src + i);
    }

    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        printf("i: %d; shared_digits[i]: %d; digits[i]: %d\n", i, *(shared_digits + i), *(src + i));
    }
    printf("-----\n");

    return 1;
}