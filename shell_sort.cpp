#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int** makeArr(int rows, int columns) {
    int** result = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; ++i) {
        result[i] = (int*)calloc(columns, sizeof(int));
    }
    return result;
}

void fillRand(int** arrays, int rows, int columns) {
    for (int i = 0; i < rows; ++i) {
        srand(time(NULL));
        for (int j = 0; j < columns; ++j) {
            arrays[i][j] = rand();
        }
    }
}

int main(int argc, char** argv) {
    // Init data
    const int arLen = 10000000;
    const int arNum = 20;

    clock_t all_time_start = clock();

    int** arrays = makeArr(arNum, arLen);
    fillRand(arrays, arNum, arLen);

    int threadCount = 8;

    double totalTime = 0;
    for (int arId = 0; arId < arNum; ++arId) {
        for (int d = arLen / 2; d > 0; d /= 2) {
            int i, j, x, k;
            #pragma omp parallel for num_threads(threadCount) shared(arrays, arId, arLen, d) private(i, j, x, k) default(none)
            for (i = 0; i < d; ++i) {
                for (j = i + d; j < arLen; j += d) {
                    x = arrays[arId][j];
                    k = j - d;
                    while (k >= 0 && arrays[arId][k] > x) {
                        arrays[arId][k + d] = arrays[arId][k];
                        k -= d;
                    }
                    arrays[arId][k + d] = x;
                }
            }
        }
        int flag = 1;
        for (int i = 1; i < arLen; ++i) {
            if (arrays[arId][i] < arrays[arId][i - 1]) {
                flag = 0;
            }
        }
        if (flag == 0) {
                fprintf(stderr, "NOT SORTED!");
        }
    }

    clock_t all_time_end = clock();
    double all_time = (double) (all_time_end - all_time_start) / CLOCKS_PER_SEC;

    printf("Seconds spent: %lf\n", all_time);
    printf("\n");
    return 0;
}
