#include <stdio.h>
#include "vector.h"

int main() {
    printf("Hello, World!\n");
    vector* myVector = malloc(sizeof(vector));

    vector__init(myVector);

    printf("%d\n", myVector->size);
    return 0;
}
