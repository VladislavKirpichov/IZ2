#include <stdio.h>
#include "vector.h"
#include "file.h"

#define MAX_FILE_NAME 255

char* get_file_name() {
    char fileName[MAX_FILE_NAME];
    printf("%s", "Input file name:\n");
    scanf("%s", fileName);

    return fileName;
}

int main() {
    char data[] = "123\n122";
    FILE* file = fmemopen(data, strlen(data), "r"); // Open string like file

    vector* myVector = vector__init_with_file(file);

    for (int i = 0; i < 5; ++i) {
        printf("%u ", myVector->arr[i]);
    }

    fclose(file);
    return 0;
}
