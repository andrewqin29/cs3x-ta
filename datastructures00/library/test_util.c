#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#include <setjmpex.h>
#include <signal.h>
#include <windows.h>
bool SIGABRT_RAISED = false;
jmp_buf env;
#else
#include <sys/wait.h>
#include <unistd.h>
#endif

bool isclose(double d1, double d2) {
    return fabs(d1 - d2) < 1e-7;
}

bool streq(const char *s1, const char *s2) {
    return strcmp(s1, s2) == 0;
}

bool packed_strarray_eq(packed_strarray_t *arr1, packed_strarray_t *arr2) {
    if (packed_strarray_len(arr1) != packed_strarray_len(arr2)) {
        return false;
    }
    for (size_t i = 0; i < packed_strarray_len(arr1); i++) {
        if (!streq(packed_strarray_get(arr1, i), packed_strarray_get(arr2, i))) {
            return false;
        }
    }
    return true;
}

bool packed_strarray_matches(packed_strarray_t *arr, const char *expected[], size_t length) {
    if (packed_strarray_len(arr) != length) {
        return false;
    }
    for (size_t i = 0; i < length; i++) {
        if (!streq(packed_strarray_get(arr, i), expected[i])) {
            return false;
        }
    }
    return true;
}

int wrap_strcmp(const void *s1, const void *s2) {
    return strcmp(* (char **) s1, * (char **) s2);
}

bool packed_strarray_set_matches(packed_strarray_t *arr, const char *expected[], size_t length) {
    if (packed_strarray_len(arr) != length) {
        return false;
    }
    char **exp_sort = malloc(sizeof(char *) * length);
    memcpy(exp_sort, expected, sizeof(char *) * length);
    qsort(exp_sort, length, sizeof(char *), wrap_strcmp);
    char **act_sort = malloc(sizeof(char *) * length);
    for (size_t i = 0; i < packed_strarray_len(arr); i++) {
        act_sort[i] = (char *) packed_strarray_get(arr, i);
    }
    qsort(act_sort, length, sizeof(char *), wrap_strcmp);
    bool ret = true;
    for (size_t i = 0; i < length; i++) {
        if (!streq(exp_sort[i], act_sort[i])) {
            ret = false;
            break;
        }
    }
    free(exp_sort);
    free(act_sort);
    return ret;
}

packed_strarray_t *packed_strarray_from_array(const char *arr[], size_t length) {
    packed_strarray_t *strarr = packed_strarray_init((char **) arr, length);
    return strarr;
}

void read_testname(char *filename, char *testname, size_t testname_size) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Couldn't open file %s\n", filename);
        exit(1);
    }
    // Generate format string
    char fmt[12];
    snprintf(fmt, sizeof(fmt), "%%%lus", (unsigned long) testname_size - 1);
    fscanf(f, fmt, testname);
    fclose(f);
}

bool contains_any(char **testnames, char *test) {
    for (size_t i = 0; testnames[i] != NULL; i++) {
        if (strstr(test, testnames[i]) != NULL) {
            return true;
        }
    }
    return false;
}

#ifdef _WIN32
void signal_handler(int signum) {
    if (signum == SIGABRT) {
        SIGABRT_RAISED = true;
        // Unregister self, and tell kernel to ignore
        signal(signum, SIG_DFL);

        // Jump out to avoid abort call after signal_handler returns
        // Safe-ish, since our code is (probably) dead simple?
        // Some comments
        // https://gist.github.com/kekyo/cc9bace942b8c2aa2484431e047d267d
        // https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/signal?view=vs-2019#remarks
        longjmp(env, 1);
    }
}
#endif

bool test_assert_fail(void (*run)(void *aux), void *aux) {
// Windows can't use POSIX apis
#ifndef _WIN32
    if (fork()) { // parent process
        int *status = malloc(sizeof(*status));
        assert(status != NULL);
        wait(status);
        // Check whether child process aborted
        bool aborted = WIFSIGNALED(*status) && WTERMSIG(*status) == SIGABRT;
        free(status);
        return aborted;
    } else {                             // child process
        freopen("/dev/null", "w", stderr); // suppress assertion message
        run(aux);
        exit(0); // should not be reached
    }
#else
    // NOTE: this is super sketch. A "better" alternative may be to just not run
    // at all.

    // Register signal handler to trap SIGABRT from assert
    signal(SIGABRT, signal_handler);
    SIGABRT_RAISED = false;
    // Store another file descriptor for stderr so that we can get it back later
    int cstderr = _dup(_fileno(stderr));

    // Run expected failure, jumping back when SIGABRT is raised
    if (setjmp(env) == 0) {
        // Suppress assertion message by piping to NUL
        freopen("NUL", "w", stderr);
        run(aux);
    }

    // Since we are in the same process, undo the suppression by reassigning
    // stderr to the saved descriptor
    _dup2(cstderr, _fileno(stderr));

    // Return whether we failed or not.
    return SIGABRT_RAISED;
#endif
}

bool test_assert_fail_packed_strarray_get(packed_strarray_t *arr, size_t idx) {
// Windows can't use POSIX apis
#ifndef _WIN32
    if (fork()) { // parent process
        int *status = malloc(sizeof(*status));
        assert(status != NULL);
        wait(status);
        // Check whether child process aborted
        bool aborted = WIFSIGNALED(*status) && WTERMSIG(*status) == SIGABRT;
        free(status);
        return aborted;
    } else {                             // child process
        freopen("/dev/null", "w", stderr); // suppress assertion message
        packed_strarray_get(arr, idx);
        exit(0); // should not be reached
    }
#else
    // NOTE: this is super sketch. A "better" alternative may be to just not run
    // at all.

    // Register signal handler to trap SIGABRT from assert
    signal(SIGABRT, signal_handler);
    SIGABRT_RAISED = false;
    // Store another file descriptor for stderr so that we can get it back later
    int cstderr = _dup(_fileno(stderr));

    // Run expected failure, jumping back when SIGABRT is raised
    if (setjmp(env) == 0) {
        // Suppress assertion message by piping to NUL
        freopen("NUL", "w", stderr);
        packed_strarray_get(arr, idx);
    }

    // Since we are in the same process, undo the suppression by reassigning
    // stderr to the saved descriptor
    _dup2(cstderr, _fileno(stderr));

    // Return whether we failed or not.
    return SIGABRT_RAISED;
#endif
}

bool test_assert_fail_packed_generic_array_get(packed_generic_array_t *arr, size_t idx) {
// Windows can't use POSIX apis
#ifndef _WIN32
    if (fork()) { // parent process
        int *status = malloc(sizeof(*status));
        assert(status != NULL);
        wait(status);
        // Check whether child process aborted
        bool aborted = WIFSIGNALED(*status) && WTERMSIG(*status) == SIGABRT;
        free(status);
        return aborted;
    } else {                             // child process
        freopen("/dev/null", "w", stderr); // suppress assertion message
        packed_generic_array_get(arr, idx);
        exit(0); // should not be reached
    }
#else
    // NOTE: this is super sketch. A "better" alternative may be to just not run
    // at all.

    // Register signal handler to trap SIGABRT from assert
    signal(SIGABRT, signal_handler);
    SIGABRT_RAISED = false;
    // Store another file descriptor for stderr so that we can get it back later
    int cstderr = _dup(_fileno(stderr));

    // Run expected failure, jumping back when SIGABRT is raised
    if (setjmp(env) == 0) {
        // Suppress assertion message by piping to NUL
        freopen("NUL", "w", stderr);
        packed_generic_array_get(arr, idx);
    }

    // Since we are in the same process, undo the suppression by reassigning
    // stderr to the saved descriptor
    _dup2(cstderr, _fileno(stderr));

    // Return whether we failed or not.
    return SIGABRT_RAISED;
#endif
}