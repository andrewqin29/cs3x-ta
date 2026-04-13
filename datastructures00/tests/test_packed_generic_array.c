#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <test_util.h>
#include <packed_generic_array.h>

typedef struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

void test_packed_generic_array_simple() {
    uint8_t a = 5;
    uint8_t *input[] = {&a};
    packed_generic_array_t *arr = packed_generic_array_init((void **) input, 1, sizeof(uint8_t));
    assert(5 == *(uint8_t *)packed_generic_array_get(arr, 0));
    assert(!test_assert_fail_packed_generic_array_get(arr, 0));
    assert(test_assert_fail_packed_generic_array_get(arr, -1));
    assert(test_assert_fail_packed_generic_array_get(arr, 10));
    assert(packed_generic_array_len(arr) == 1);
    packed_generic_array_free(arr);

    rgb_t color = {2, 1, 4};
    rgb_t *input1[] = {&color};
    arr = packed_generic_array_init((void **) input1, 1, sizeof(rgb_t));
    rgb_t *temp = (rgb_t *)packed_generic_array_get(arr, 0);
    assert(temp->r == color.r && temp->g == color.g && temp->b == color.b);
    packed_generic_array_free(arr);

    double b = 1.3f;
    double *input2[] = {&b};
    arr = packed_generic_array_init((void **) input2, 1, sizeof(double));
    assert(1.3f == *(double *)packed_generic_array_get(arr, 0));
    packed_generic_array_free(arr);
}

void test_packed_generic_array_small() {
    uint8_t a = 1;
    uint8_t b = 2;
    uint8_t c = 3;
    uint8_t *input[] = {&a, &b, &c};
    packed_generic_array_t *arr = packed_generic_array_init((void **) input, 3, sizeof(uint8_t));

    assert(1 == *(uint8_t *)packed_generic_array_get(arr, 0));
    assert(3 == *(uint8_t *)packed_generic_array_get(arr, 2));
    assert(2 == *(uint8_t *)packed_generic_array_get(arr, 1));
    assert(packed_generic_array_len(arr) == 3);
    packed_generic_array_free(arr);

    rgb_t color = {2, 1, 4};
    rgb_t color1 = {3, 4, 5};
    rgb_t color2 = {6, 7, 8};
    rgb_t *input1[] = {&color, &color1, &color2};
    arr = packed_generic_array_init((void **) input1, 3, sizeof(rgb_t));

    rgb_t *temp = (rgb_t *)packed_generic_array_get(arr, 0);
    assert(temp->r == color.r && temp->g == color.g && temp->b == color.b);
    temp = (rgb_t *)packed_generic_array_get(arr, 2);
    assert(temp->r == color2.r && temp->g == color2.g && temp->b == color2.b);
    temp = (rgb_t *)packed_generic_array_get(arr, 1);
    assert(temp->r == color1.r && temp->g == color1.g && temp->b == color1.b);
    packed_generic_array_free(arr);

    double d = 1.5f;
    double e = 2.7f;
    double f = -1.2f;
    double *input2[] = {&d, &e, &f};
    arr = packed_generic_array_init((void **) input2, 3, sizeof(double));
    assert(1.5f == *(double *)packed_generic_array_get(arr, 0));
    assert(-1.2f == *(double *)packed_generic_array_get(arr, 2));
    assert(2.7f == *(double *)packed_generic_array_get(arr, 1));
    packed_generic_array_free(arr);
}

#define LARGE_SIZE 100
size_t count_digits(size_t number) {
    if (number == 0) return 1;
    size_t count = 0;

    if (number < 0) number *= -1;

    while (number != 0) {
        number /= 10;
        count++;
    }

    return count;
}

bool check_result(rgb_t *result, size_t idx) {
    return result->r == idx && result->g == idx * 2 && result->b == idx + 4;
}

void test_packed_generic_array_large() {
    uint8_t **input = malloc(8 * LARGE_SIZE);
    for (size_t i = 0; i < LARGE_SIZE; i++) {
        uint8_t *temp = malloc(sizeof(uint8_t));
        *temp = i;
        input[i] = temp;
    }
    packed_generic_array_t *arr = packed_generic_array_init((void **) input, LARGE_SIZE, sizeof(uint8_t));
    for (size_t i = 0; i < LARGE_SIZE; i++) {
        free(input[i]);
    }
    free(input);
    for (size_t i = 0; i < LARGE_SIZE; ++i) {
        assert(i == *(uint8_t *) packed_generic_array_get(arr, i));
    }
    assert(packed_generic_array_len(arr) == 100);

    packed_generic_array_free(arr);

    rgb_t **input1 = malloc(8 * LARGE_SIZE);
    for (size_t i = 0; i < LARGE_SIZE; i++) {
        rgb_t *temp = malloc(sizeof(rgb_t));
        temp->r = i;
        temp->g = i * 2;
        temp->b = i + 4;
        input1[i] = temp;
    }
    arr = packed_generic_array_init((void **) input1, LARGE_SIZE, sizeof(rgb_t));
    for (size_t i = 0; i < LARGE_SIZE; i++) {
        free(input1[i]);
    }
    free(input1);
    for (size_t i = 0; i < LARGE_SIZE; ++i) {
        assert(check_result((rgb_t *) packed_generic_array_get(arr, i), i));
    }
    packed_generic_array_free(arr);
}

void test_packed_generic_array_empty() {
    uint8_t *input[] = {};
    packed_generic_array_t *arr = packed_generic_array_init((void **) input, 0, sizeof(uint8_t));

    assert(test_assert_fail_packed_generic_array_get(arr, 0));
    assert(test_assert_fail_packed_generic_array_get(arr, 100));
    packed_generic_array_free(arr);
    
    rgb_t *input1[] = {};
    arr = packed_generic_array_init((void **) input1, 0, sizeof(rgb_t));

    assert(test_assert_fail_packed_generic_array_get(arr, 0));
    assert(test_assert_fail_packed_generic_array_get(arr, 100));
    packed_generic_array_free(arr);

    double *input2[] = {};
    arr = packed_generic_array_init((void **) input2, 0, sizeof(rgb_t));

    assert(test_assert_fail_packed_generic_array_get(arr, 0));
    assert(test_assert_fail_packed_generic_array_get(arr, 100));
    packed_generic_array_free(arr);
}

void test_packed_generic_array_check_contiguous() {
    uint8_t a = 1;
    uint8_t b = 2;
    uint8_t c = 3;
    uint8_t *input[] = {&a, &b, &c};
    packed_generic_array_t *arr = packed_generic_array_init((void **) input, 3, sizeof(uint8_t));

    uint8_t *temp = (uint8_t *)packed_generic_array_get(arr, 0);

    assert(1 == *temp);
    assert(3 == *(temp + 2));
    assert(2 == *(temp + 1));
    assert(packed_generic_array_len(arr) == 3);
    packed_generic_array_free(arr);

    rgb_t color = {2, 1, 4};
    rgb_t color1 = {3, 4, 5};
    rgb_t color2 = {6, 7, 8};
    rgb_t *input1[] = {&color, &color1, &color2};
    arr = packed_generic_array_init((void **) input1, 3, sizeof(rgb_t));

    rgb_t *temp1 = (rgb_t *)packed_generic_array_get(arr, 0);
    assert(temp1->r == color.r && temp1->g == color.g && temp1->b == color.b);
    temp1 += 2;
    assert(temp1->r == color2.r && temp1->g == color2.g && temp1->b == color2.b);
    temp1--;
    assert(temp1->r == color1.r && temp1->g == color1.g && temp1->b == color1.b);
    packed_generic_array_free(arr);

    double d = 1.5f;
    double e = 2.7f;
    double f = -1.2f;
    double *input2[] = {&d, &e, &f};
    arr = packed_generic_array_init((void **) input2, 3, sizeof(double));
    double *temp2 = (double *)packed_generic_array_get(arr, 0);
    assert(1.5f == *temp2);
    assert(-1.2f == *(temp2 + 2));
    assert(2.7f == *(temp2 + 1));
    packed_generic_array_free(arr);
}

void test_packed_generic_array_check_not_copy() {
    uint8_t a = 5;
    uint8_t *input[] = {&a};
    packed_generic_array_t *arr = packed_generic_array_init((void **) input, 1, sizeof(uint8_t));
    uint8_t *temp = (uint8_t *) packed_generic_array_get(arr, 0);
    *temp = 6;
    temp = (uint8_t *) packed_generic_array_get(arr, 0);
    assert(6 == *temp);
    
    packed_generic_array_free(arr);

    rgb_t color = {2, 1, 4};
    rgb_t *input1[] = {&color};
    arr = packed_generic_array_init((void **) input1, 1, sizeof(rgb_t));
    rgb_t *temp1 = (rgb_t *) packed_generic_array_get(arr, 0);
    temp1->r = 7;
    temp1->g = 5;
    temp1->b = 6;
    temp1 = (rgb_t *) packed_generic_array_get(arr, 0);
    assert(temp1->r == 7 && temp1->g == 5 && temp1->b == 6);
    packed_generic_array_free(arr);

    double b = 1.3f;
    double *input2[] = {&b};
    arr = packed_generic_array_init((void **) input2, 1, sizeof(double));
    double *temp2 = (double *) packed_generic_array_get(arr, 0);
    *temp2 = -123.4f;
    temp2 = (double *) packed_generic_array_get(arr, 0);
    assert(-123.4f == *temp2);
    packed_generic_array_free(arr);
}

int main(int argc, char *argv[]) {
    // Run all tests? True if there are no command-line arguments
    bool all_tests = argc == 1;
    char **testnames = argv + 1;
    
    DO_TEST(test_packed_generic_array_simple)
    DO_TEST(test_packed_generic_array_small)
    DO_TEST(test_packed_generic_array_large)
    DO_TEST(test_packed_generic_array_empty)
    DO_TEST(test_packed_generic_array_check_contiguous)
    DO_TEST(test_packed_generic_array_check_not_copy)
    puts("packed_generic_array_test PASS");
}