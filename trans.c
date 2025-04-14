/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int r;
    int c;
    for (r = 0; r < N - (N % 8); r += 8) { // r = start row of block in A
        for (c = 0; c < M - (M % 8); c += 8) { // c = start col of block in A
                int a0, a1, a2, a3, a4, a5, a6, a7;
                int diag;
                for (int i = r; i < r + 8; ++i) {
                    a0 = A[i][c + 0];
                    a1 = A[i][c + 1];
                    a2 = A[i][c + 2];
                    a3 = A[i][c + 3];
                    a4 = A[i][c + 4];
                    a5 = A[i][c + 5];
                    a6 = A[i][c + 6];
                    a7 = A[i][c + 7];

                    if (c + 0 == i) diag = a0; else B[c + 0][i] = a0;
                    if (c + 1 == i) diag = a1; else B[c + 1][i] = a1;
                    if (c + 2 == i) diag = a2; else B[c + 2][i] = a2;
                    if (c + 3 == i) diag = a3; else B[c + 3][i] = a3;
                    if (c + 4 == i) diag = a4; else B[c + 4][i] = a4;
                    if (c + 5 == i) diag = a5; else B[c + 5][i] = a5;
                    if (c + 6 == i) diag = a6; else B[c + 6][i] = a6;
                    if (c + 7 == i) diag = a7; else B[c + 7][i] = a7;
                    if (r == c) B[i][i] = diag;
                }
        }
    }

    // Process remaining columns for the main bulk of rows
    for (r = 0; r < N - (N % 8); ++r) {
        for (c = M - (M % 8); c < M; ++c) {
            B[c][r] = A[r][c];
        }
    }

    // Process remaining full rows
    for (r = N - (N % 8); r < N; ++r) {
        for (c = 0; c < M; ++c) {
            B[c][r] = A[r][c];
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 
char otherdude_desc[] = "Whatever amn";
void otherdude(int M, int N, int A[N][M], int B[M][N])
{
    int a_0, a_1, a_2, a_3, a_4, a_5, a_6, a_7;
    for (int i = 0; i < 64; i += 8)
    {
        for (int j = 0; j < 64; j += 8)
        {
            for (int k = i; k < i + 4; k++)
            {
                a_0 = A[k][j + 0];
                a_1 = A[k][j + 1];
                a_2 = A[k][j + 2];
                a_3 = A[k][j + 3];
                a_4 = A[k][j + 4];
                a_5 = A[k][j + 5];
                a_6 = A[k][j + 6];
                a_7 = A[k][j + 7];

                B[j + 0][k] = a_0;
                B[j + 1][k] = a_1;
                B[j + 2][k] = a_2;
                B[j + 3][k] = a_3;
                B[j + 0][k + 4] = a_4;
                B[j + 1][k + 4] = a_5;
                B[j + 2][k + 4] = a_6;
                B[j + 3][k + 4] = a_7;
            }
            for (int k = j; k < j + 4; k++)
            {
                a_0 = B[k][i + 4];
                a_1 = B[k][i + 5];
                a_2 = B[k][i + 6];
                a_3 = B[k][i + 7];

                a_4 = A[i + 4][k];
                a_5 = A[i + 5][k];
                a_6 = A[i + 6][k];
                a_7 = A[i + 7][k];

                B[k][i + 4] = a_4;
                B[k][i + 5] = a_5;
                B[k][i + 6] = a_6;
                B[k][i + 7] = a_7;

                B[k + 4][i + 0] = a_0;
                B[k + 4][i + 1] = a_1;
                B[k + 4][i + 2] = a_2;
                B[k + 4][i + 3] = a_3;
            }
            for (int k = i + 4; k < i + 8; k++)
            {
                a_4 = A[k][j + 4];
                a_5 = A[k][j + 5];
                a_6 = A[k][j + 6];
                a_7 = A[k][j + 7];

                B[j + 4][k] = a_4;
                B[j + 5][k] = a_5;
                B[j + 6][k] = a_6;
                B[j + 7][k] = a_7;
            }
        }
    }
}

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */ 

    /* Register any additional transpose functions */
    registerTransFunction(otherdude, otherdude_desc);

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

