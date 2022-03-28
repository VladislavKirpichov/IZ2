#include "histogram.h"

#define NUMBER_OF_DIGITS 10

int* hist(const int const* nums, size_t* digits, size_t size) {
    if (nums == NULL) {
        perror("NULL nums");
        return -1;
    }

    if (size == 0) {
        perror("nums is empty!");
        return 0;
    }
    int temp = 0;

    for (size_t i = 0; i < size; ++i) {
        temp = nums[i];

        while (temp > 0) {
            digits[temp % 10] += 1;
            temp /= 10;
        }
    }

    return 1;
}

int create_hist(const int const* nums, size_t size) {
    size_t digits[NUMBER_OF_DIGITS] = {0};
    if (!hist(nums, digits, size))
        return 0;

    printf("%s\n", "--- HISTOGRAM ---");

    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        printf("[%d] ", i);
        for (int j = 0; j < digits[i]; ++j) {
            printf("*");
        }
        printf("\n");
    }
}