#include "histogram.h"

#define NUMBER_OF_DIGITS 10

int hist(const int* nums, size_t* digits, size_t size) {
    if (nums == NULL || digits == NULL) {
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

int create_hist(const int* nums, FILE* output, size_t size) {
    size_t digits[NUMBER_OF_DIGITS] = {0};

    if (!hist(nums, digits, size))
        return -1;

     /* ---- print in output (can be stdin) ---- */

    fprintf(output, "%s\n", "--- HISTOGRAM ---");

    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        fprintf(output, "[%d] %ld\n", i, *(digits + i));
    }

    int med = median(digits);
    fprintf(output, "Median: %d\n", med);

    return med;
}

int median(size_t* digits) {
    
    size_t sum = 0;
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        sum += digits[i];
    }

    size_t temp = 0;
    for (int i = 0; i < NUMBER_OF_DIGITS; ++i) {
        temp += digits[i];

        if (temp > sum / 2) {
            return i;
        }
    }

    return NUMBER_OF_DIGITS - 1;
}
