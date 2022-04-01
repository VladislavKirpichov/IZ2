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
    printf("%s", "Input file name (from ./data):\n");
    scanf("%s", fileName);
}

int main() {
    char fileName[MAX_FILE_NAME];
    get_file_name(fileName);
    // strcpy(fileName, "4gb.txt");
    clock_t start, end;

    char data[] = "./data/";
    FILE* file = fopen(strcat(data, fileName), "r");

    start = clock();

    vector* myVector = vector__init_with_file(file);

    if (myVector == NULL) {
        perror("vector__init error");
        return 0;
    }

    create_hist(myVector->arr, myVector->size);

    end = clock();

    printf("Time spent: %ld seconds\n", (end - start) / CLOCKS_PER_SEC);

    fclose(file);
    vector__free(myVector);
    return 0;
}
