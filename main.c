#include <stdio.h>
#include <string.h>
#include "vector.h"
#include "file.h"
#include "histogram.h"
#include <time.h>

#define MAX_FILE_NAME 255

void get_file_name(char fileName[MAX_FILE_NAME]) {
    printf("%s", "Input file name (from ./data):\n");
    scanf("%s", fileName);
}

int main() {
    char fileName[MAX_FILE_NAME];
    // get_file_name(fileName);
    strcpy(fileName, "225kb.txt");

    char data[] = "./data/";

    FILE* file = fopen(strcat(data, fileName), "r");

    vector* myVector = vector__init_with_file(file);

    if (myVector == NULL) {
        perror("vector__init error");
        return 0;
    }

    create_hist(myVector->arr, myVector->size);

    fclose(file);
    vector__free(myVector);
    return 0;
}
