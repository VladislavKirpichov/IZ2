#include <gtest/gtest.h>
#include <stdio.h>

extern "C" {
    #include "vector.h"
    #include "file.h"
    #include "histogram.h"
    #include "shared_memory.h"
}

size_t fsize(FILE* file) {
    fseek(file, 0, SEEK_SET);
    size_t size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

bool check_if_eq_files(FILE* file1, FILE* file2) {

    if (fsize(file1) != fsize(file2)) {
        printf("fsize(file1) != fsize(file2)");
        return false;
    }

    fseek(file1, 0, SEEK_SET);
    fseek(file2, 0, SEEK_SET);

    char ch1 = fgetc(file1);
    char ch2 = fgetc(file2);

    size_t size = 0;
    while ((ch1 = fgetc(file1)) != EOF &&
            (ch2 = fgetc(file2)) != EOF) {
        if (ch1 != ch2) {
            fseek(file1, 0, SEEK_SET);
            fseek(file2, 0, SEEK_SET);
            return false;
        }
    }

    fseek(file1, 0, SEEK_SET);
    fseek(file2, 0, SEEK_SET);
    return true;
}

TEST(file, chunk) {
    EXPECT_EQ(1024 * 4 * 16, chunk_size(1024));
}

TEST(GetDataFromFile, small_data) {
    char buffer[] = "123\n123\n123";
    FILE* file = fmemopen(buffer, 12, "r");

    int answer[] = { 123, 123, 123 };
    int* arr;
    EXPECT_EQ(3, get_data_from_file(&arr, file));
    for (int i = 0; i < 3; ++i) {
        EXPECT_TRUE(arr[i] == answer[i]);
    }

    shared_free(arr, 3);
    fclose(file);
}

TEST(Histogram, hist_test_small__1) {
    int nums[] = { 1 };
    size_t answer[] = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0 };

    size_t* digits = (size_t*)malloc(10 * sizeof(size_t));
    for (int i = 0; i < 10; ++i) {
        *(digits + i) = 0;
    }
    hist(nums, digits, 0, 1);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(answer[i], digits[i]);
    }

    free(digits);
}

TEST(Histogram, hist_test_small__2) {
    int nums[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    size_t answer[] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    size_t* digits = (size_t*)malloc(10 * sizeof(size_t));
    for (int i = 0; i < 10; ++i) {
        *(digits + i) = 0;
    }
    hist(nums, digits, 0, 9);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(answer[i], digits[i]);
    }

    free(digits);
}

TEST(Histogram, hist_test_medium) {
    int nums[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    1111111111, 1111111111, 1111111111, 1111111111,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

    size_t answer[] = {0, 40, 0, 0, 0, 0, 0, 0, 0, 0 };

    size_t* digits = (size_t*)malloc(10 * sizeof(size_t));
    for (int i = 0; i < 10; ++i) {
        *(digits + i) = 0;
    }
    hist(nums, digits, 0, 289);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(answer[i], digits[i]);
    }

    free(digits);
}

TEST(Histogram, hist_test_not_valid_input) {
    int nums[] = {1};
    size_t answer[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    size_t* digits = (size_t*)malloc(10 * sizeof(size_t));
    for (int i = 0; i < 10; ++i) {
        *(digits + i) = 0;
    }

    EXPECT_EQ(-1, hist(NULL, digits, 0, 10));
    EXPECT_EQ(-1, hist(nums, NULL, 0, 10));
    
    hist(nums, digits, 1, 0);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(answer[i], digits[i]);
    }

    free(digits);
}

TEST(General, input_small) {
    FILE* input = fopen("./tests/input/test1.txt", "r");
    FILE* answer = fopen("./tests/answers/answer1.txt", "r");

    vector* tVector = vector__init_with_file(input);    // test vector

    FILE* output = fopen("output.txt", "w+");
    create_hist(tVector->arr, output, tVector->size);

    EXPECT_EQ(true, check_if_eq_files(output, answer));

    fclose(input);
    fclose(answer);
    fclose(output);
    vector__free(tVector);
}

TEST(General, input_small__1) {
    FILE* input = fopen("./tests/input/test1.txt", "r");
    FILE* answer = fopen("./tests/answers/answer1.txt", "r");

    vector* tVector = vector__init_with_file(input);    // test vector

    FILE* output = fopen("output.txt", "w+");
    create_hist(tVector->arr, output, tVector->size);

    EXPECT_EQ(true, check_if_eq_files(output, answer));

    fclose(input);
    fclose(answer);
    fclose(output);
    vector__free(tVector);
}

TEST(General, input_small__2) {
    FILE* input = fopen("./tests/input/test2.txt", "r");
    FILE* answer = fopen("./tests/answers/answer2.txt", "r");

    vector* tVector = vector__init_with_file(input);    // test vector

    FILE* output = fopen("output.txt", "w+");
    create_hist(tVector->arr, output, tVector->size);

    EXPECT_EQ(true, check_if_eq_files(output, answer));

    fclose(input);
    fclose(answer);
    fclose(output);
    vector__free(tVector);
}

TEST(GeneralStress, stress__1mb) {
    FILE* input = fopen("./tests/input/stest_1mb.txt", "r");
    FILE* answer = fopen("./tests/answers/sanswer_1mb.txt", "r");

    vector* tVector = vector__init_with_file(input);    // test vector

    FILE* output = fopen("output.txt", "w+");
    create_hist(tVector->arr, output, tVector->size);

    EXPECT_EQ(true, check_if_eq_files(output, answer));

    fclose(input);
    fclose(answer);
    fclose(output);
    vector__free(tVector);
}

TEST(GeneralStress, stress__100mb) {
    FILE* input = fopen("./tests/input/stest_100mb.txt", "r");
    FILE* answer = fopen("./tests/answers/sanswer_100mb.txt", "r");

    vector* tVector = vector__init_with_file(input);    // test vector

    FILE* output = fopen("output.txt", "w+");
    create_hist(tVector->arr, output, tVector->size);

    EXPECT_EQ(true, check_if_eq_files(output, answer));

    fclose(input);
    fclose(answer);
    fclose(output);
    vector__free(tVector);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
