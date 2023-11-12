#include <time.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "ReadStrings.h"
#include "KaratsubaMul.h"

int test_karatsuba(char *main_test_name, char *result_file_name);

int read_test_karatsuba_data(const char * const filename, int* * coef_a, int* * coef_b, int* * answers, int *N);

int check_answers(FILE* resultfile, int* coef_ans, int* answers, const int N);

void reset_to_zero(int *arr, int n);

int main(int argv, char * *argc) {
    if (argv != 3) {
        printf("Incorrect enter\n");
        return -1;
    }

    printf("%d\n", test_karatsuba(argc[1], argc[2]));

    return 0;
}

int test_karatsuba(char *main_test_name, char *result_file_name) {
    FILE* result_file = fopen(result_file_name, "w");
    if (result_file == NULL) {
        printf("failure when opening file for writing\n");
        return -1;
    }
    //for names 
    size_t num_tests = 0;
    unsigned buf_capacity = 0;
    struct string *tests_names = NULL;
    char *buf = NULL;
    struct timespec ts_last_k, ts_current_k, ts_last_u, ts_current_u;
    //for tests data
    int N = 0;
    int *coef_a = NULL;
    int *coef_b = NULL;
    int *answers = NULL;
    int *coef_ans = NULL;
    double n_log2_3 = 0;

    read_strings(&tests_names, &num_tests, &buf, main_test_name);
    change_str_ending_buf(buf);

    for (unsigned i = 0; i < num_tests; i++) {
        if (read_test_karatsuba_data(tests_names[i].str, &coef_a, &coef_b, &answers, &N)) {
            printf("read_test_timsort_data failure\n");
            fclose(result_file);
            return -1;
        }

        coef_ans = (int*) calloc(N + N - 1, sizeof(int));

        timespec_get(&ts_last_k, TIME_UTC);
        karatsuba_multiply(coef_ans, coef_a, coef_b, N); //karatsuba_multiply
        timespec_get(&ts_current_k, TIME_UTC);

        n_log2_3 = pow(N, log2(3));
    
        fprintf(result_file, "TEST FROM FILE %s: {\n\n", tests_names[i].str);
        fprintf(result_file, "\tKaratsuba multiplation:\n");
        fprintf(result_file, "\t|------------------------------------------------\n");
        fprintf(result_file, "\t| N = %d    |  N^(log2(3)) = %lf\n", N, n_log2_3);
        fprintf(result_file, "\t|------------------------------------------------\n");
        fprintf(result_file, "\t| Execution time: %lu.%.09ld \n", ts_current_k.tv_sec - ts_last_k.tv_sec, ts_current_k.tv_nsec - ts_last_k.tv_nsec);
        fprintf(result_file, "\t|------------------------------------------------\n");
        fprintf(result_file, "\t| Time / N^(log2(3)) = %lf\n", (ts_current_k.tv_nsec - ts_last_k.tv_nsec) / n_log2_3);
        fprintf(result_file, "\t|------------------------------------------------\n");

        check_answers(result_file, coef_ans, answers, N);

        fprintf(result_file, "\t|------------------------------------------------\n\n");

        fprintf(result_file, "\tUsual multiplication: \n");

        reset_to_zero(coef_ans, N + N - 1);

        timespec_get(&ts_last_u, TIME_UTC);
        multiply_polynomials(coef_ans, coef_a, coef_b, N); //usual multiply
        timespec_get(&ts_current_u, TIME_UTC);
    
        fprintf(result_file, "\t|------------------------------------------------\n");
        fprintf(result_file, "\t| N = %d       |  N * N = %d\n", N, N * N);
        fprintf(result_file, "\t|------------------------------------------------\n");
        fprintf(result_file, "\t| Execution time: %lu.%.09ld \n", ts_current_u.tv_sec - ts_last_u.tv_sec, ts_current_u.tv_nsec - ts_last_u.tv_nsec);
        fprintf(result_file, "\t|------------------------------------------------\n");
        fprintf(result_file, "\t| Time / (N * N) = %lf\n", (ts_current_u.tv_nsec - ts_last_u.tv_nsec) / pow(N, 2));
        fprintf(result_file, "\t|------------------------------------------------\n");

        check_answers(result_file, coef_ans, answers, N);

        fprintf(result_file, "\t|------------------------------------------------\n\n");

        fprintf(result_file, "\tUsual time / Karatsuba time = %lf\n", (ts_current_u.tv_nsec - ts_last_u.tv_nsec) / (double) (ts_current_k.tv_nsec - ts_last_k.tv_nsec));
        fprintf(result_file, "}\n\n");

        free(coef_ans);
        free(coef_a);
        free(coef_b);
        free(answers);
    }

    fclose(result_file);

    clean_strings(&tests_names, &buf);

    return 0;
}

int read_test_karatsuba_data(const char * const filename, int* * coef_a, int* * coef_b, int* * answers, int *N) {
    int check = 0;
    int N1 = 0;
    FILE* testfile = fopen(filename, "r");

    if (testfile == NULL) {
        printf("failure while opening a file %s for reading\n", filename);
        return -1;
    }

    check = fscanf(testfile, "%d%d", N, &N1);

    if (check != 2) {
        printf("failure while reading a file %s\n", filename);
        fclose(testfile);
        return -1;
    }

    if (*N != N1) {
        printf("Incorrect enter: *N == N1\n");
        fclose(testfile);
        return -1;
    }

    *coef_a = (int*) calloc(N1, sizeof(int));
    *coef_b = (int*) calloc(N1, sizeof(int));
    *answers = (int*) calloc(N1 + N1 - 1, sizeof(int));

    if ((coef_a == NULL) || (coef_b == NULL) || (answers == NULL)) {
        printf("failure callocation\n");
        fclose(testfile);
        return -1;
    }

    for (int i = 0; i < N1; i++)
        check += fscanf(testfile, "%d", (*coef_a + i));

    for (int i = 0; i < N1; i++)
        check += fscanf(testfile, "%d", (*coef_b + i));

    for (int i = 0; i < N1; i++)
        check += fscanf(testfile, "%d", (*answers + i));

    fclose(testfile);

    if (check != N1 * 3 + 2) {
        printf("failure while reading a file: check = %d and N1 * 3 + 2 = %d\n", check, N1 * 3 + 2);
        return -1;
    }

    return 0;
}

int check_answers(FILE* resultfile, int* coef_ans, int* answers, const int N) {
    for (unsigned i = 0; i < N; i++) {
        if (coef_ans[i] != answers[i]) {
            fprintf(resultfile, "| Incorrest ansver: coef_ans[%u] = %d, but answers[%u] = %d\n", i, coef_ans[i], i, answers[i]);
            return -1;
        }
    }

    fprintf(resultfile, "\t| OK\n");

    return 0;
}

void reset_to_zero(int *arr, int n) {
    for (int i = 0; i < n; i++) 
        arr[i] = 0;
}

