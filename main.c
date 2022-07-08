#include <stdio.h>
#include <string.h>
#include "vector.h"
#include "file.h"
#include "histogram.h"
#include <time.h>

#define MAX_FILE_NAME 255

void get_file_name(char fileName[MAX_FILE_NAME]) {
    printf("%s", "Input file name (from ./data): ");
    scanf("%s", fileName);
}

void compare_medians(int first_median, int second_median) {
    if (first_median > second_median)
        printf("The median of the first branch is greater than second:\nfirst = %d vs second = %d\n", first_median, second_median);

    else if (first_median < second_median)
        printf("The median of the second branch is greater than first:\nfirst = %d vs second = %d\n", first_median, second_median);

    else
        printf("Medians are equal: first = %d vs second = %d\n", first_median, second_median);
}

int main() {

    /* ---- Get file names ---- */

    char data[] = "./data/";
    char firstFileName[MAX_FILE_NAME];
    get_file_name(firstFileName);
    FILE* firstFile = fopen(strcat(data, firstFileName), "r");

    strcpy(data, "./data/\0");
    char secondFileName[MAX_FILE_NAME];
    get_file_name(secondFileName);
    FILE* secondFile = fopen(strcat(data, secondFileName), "r");

    clock_t start, end;

    /* ---- Main part of programm ---- */
    
    start = clock();
    
    vector* firstVector = vector__init_with_file(firstFile);
    fseek(firstFile, 0, SEEK_SET);

    vector* secondVector = vector__init_with_file(secondFile);
    fseek(secondFile, 0, SEEK_SET);

    if (firstVector == NULL || secondVector == NULL) {
        perror("vector__init error");
        return 0;
    }

    FILE* output = fopen("output.txt", "w+");
    int firstVectorMed = create_hist(firstVector->arr, output, firstVector->size);
    fseek(output, 0, SEEK_CUR);
    int secondVectorMed = create_hist(secondVector->arr, output, secondVector->size);

    compare_medians(firstVectorMed, secondVectorMed);

    end = clock();

    printf("Time spent: %ld seconds\n", (end - start) / CLOCKS_PER_SEC);

    fclose(firstFile);
    fclose(secondFile);
    fclose(output);

    vector__free(firstVector);
    vector__free(secondVector);
    return 0;
}
