#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <test_util.h>
#include <packed_strarray.h>

void test_packed_strarray_simple() {
    char *input[] = {"hello"};
    packed_strarray_t *arr = packed_strarray_init(input, 1);
    assert_streq("hello", (char *) packed_strarray_get(arr, 0));
    assert(!test_assert_fail_packed_strarray_get(arr, 0));
    assert(test_assert_fail_packed_strarray_get(arr, -1));
    assert(test_assert_fail_packed_strarray_get(arr, 10));
    assert(packed_strarray_len(arr) == 1);
    packed_strarray_free(arr);
}

void test_packed_strarray_small() {
    char *input[] = {"design", "blank", "2"};
    packed_strarray_t *arr = packed_strarray_init(input, 3);

    assert_streq("design", (char *) packed_strarray_get(arr, 0));
    assert_streq("2", (char *) packed_strarray_get(arr, 2));
    assert_streq("blank", (char *) packed_strarray_get(arr, 1));
    assert(packed_strarray_len(arr) == 3);
    packed_strarray_free(arr);
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

void test_packed_strarray_large() {
    char **input = malloc(LARGE_SIZE * 8);
    for (size_t i = 0; i < LARGE_SIZE; i++) {
        size_t count = count_digits(i);
        char *value = malloc(count + 2);
        snprintf(value, count + 2, "v%zu", i);
        input[i] = value;
    }
    packed_strarray_t *arr = packed_strarray_init(input, LARGE_SIZE);
    for (size_t i = 0; i < LARGE_SIZE; i++) {
        free(input[i]);
    }
    free(input);
    for (size_t i = 0; i < LARGE_SIZE; ++i) {
        size_t count = count_digits(i);
        char value[count + 2];
        snprintf(value, count + 2, "v%zu", i);
        assert_streq(value, (char *) packed_strarray_get(arr, i));
    }
    assert(packed_strarray_len(arr) == 100);

    packed_strarray_free(arr);
}

void test_packed_strarray_empty() {
    char *input[] = {};
    packed_strarray_t *arr = packed_strarray_init(input, 0);

    assert(test_assert_fail_packed_strarray_get(arr, 0));
    assert(test_assert_fail_packed_strarray_get(arr, 100));
    packed_strarray_free(arr);
}

void test_packed_strarray_check_contiguous() {
    char *input[] = {"design", "blank", "2"};
    packed_strarray_t *arr = packed_strarray_init(input, 3);

    char *temp = (char *) packed_strarray_get(arr, 0);
    assert_streq(temp, "design");
    assert_streq(temp + 7, "blank");
    assert_streq(temp + 7 + 6, "2");
    packed_strarray_free(arr);
}

void test_packed_strarray_check_not_copy() {
    char *input[] = {"design", "blank", "2"};
    packed_strarray_t *arr = packed_strarray_init(input, 3);

    char *temp = (char *) packed_strarray_get(arr, 0);
    *temp = 'c';
    temp = (char *) packed_strarray_get(arr, 0);
    assert_streq(temp, "cesign");
    packed_strarray_free(arr);
}

int main(int argc, char *argv[]) {
    // Run all tests? True if there are no command-line arguments
    bool all_tests = argc == 1;
    char **testnames = argv + 1;
    
    DO_TEST(test_packed_strarray_simple)
    DO_TEST(test_packed_strarray_small)
    DO_TEST(test_packed_strarray_large)
    DO_TEST(test_packed_strarray_empty)
    DO_TEST(test_packed_strarray_check_contiguous)
    DO_TEST(test_packed_strarray_check_not_copy)
    puts("packed_strarray_test PASS");
}