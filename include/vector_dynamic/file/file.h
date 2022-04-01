//  This file describes the methods of working with files for the vector

/*  Алгоритм работы с файлами

**  Каждый файл имеет структуру с строками размером 16кБ

**  2) Для каждого процесса также создается массив digits[NUMBER_OF_DIGITS]
**     в который данный процесс будет записывать считанные данные

**  3) После завершения работы всех процессов массивы digits из них объединяются в один
**     (достаточно просто проссумировать значения в одинаковых индексах массивов)
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "shared_memory.h"

size_t size_of_file(FILE* file); // returns number of bytes in file
size_t chuks_size(size_t size);

int get_data_from_file(int** nums, FILE* file);
int make_number_from_chars(const char** buffer);

int inner_process_file_logic(int** nums, FILE* file, size_t offset);
int fetch(int** shared_memory, size_t shared_memory_size,
          const int const* src, size_t src_size);
