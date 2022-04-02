#include <stdio.h>
#include <string.h>
#include "vector.h"
#include "file.h"
#include "histogram.h"
#include <time.h>

#define MAX_FILE_NAME 255

void create_file(FILE** file, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = rand() % 8; j < 9; ++i, ++j) {
            fprintf(*file, "%d", rand() % 10);
        }

        fprintf(*file, "\n");
    }
}

void get_file_name(char fileName[MAX_FILE_NAME]) {
    printf("%s", "Input file name (from ./data): ");
    scanf("%s", fileName);
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
    create_hist(firstVector->arr, output, firstVector->size);
    fseek(output, 0, SEEK_CUR);
    create_hist(secondVector->arr, output, secondVector->size);

    end = clock();

    printf("Time spent: %ld seconds\n", (end - start) / CLOCKS_PER_SEC);

    fclose(firstFile);
    fclose(secondFile);
    fclose(output);

    vector__free(firstVector);
    vector__free(secondVector);
    return 0;
}
