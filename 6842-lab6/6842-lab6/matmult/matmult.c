#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define Item(X, I, J, NCOL) X[((J) + (I) * (NCOL))]

typedef struct
{
    int *A; // matrix A[l,m]
    int *B; // matrix B[m,n]
    int *C; // matrix C[l,n]
    int l;  // rows of A
    int m;  // columns of A and rows of B
    int n;  // columns of B
} matrix;

matrix data;

void *v1(void *row_column)
{
    int currentRow = ((int *)row_column)[0];
    int currentColumn = ((int *)row_column)[1];

    int sum = 0;
    for (int k = 0; k < data.m; k++)
        sum += Item(data.A, currentRow, k, data.m) * Item(data.B, k, currentColumn, data.n);
    Item(data.C, currentRow, currentColumn, data.n) = sum;
}

void *v2(void *row)
{
    int currentRow = *((int *)row);
    for (int j = 0; j < data.n; j++)
    {
        int sum = 0;
        for (int k = 0; k < data.m; k++)
            sum += Item(data.A, currentRow, k, data.m) * Item(data.B, k, j, data.n);
        Item(data.C, currentRow, j, data.n) = sum;
    }
}

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult(int *A, int *B, int *C, int l, int m, int n)
{
    for (int i = 0; i < l; i++)
        for (int j = 0; j < n; j++)
        {
            int sum = 0;
            for (int k = 0; k < m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }
}

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult_v1(int *A, int *B, int *C, int l, int m, int n)
{
    data.l = l;
    data.m = m;
    data.n = n;
    data.A = A;
    data.B = B;
    data.C = (int *)malloc(data.l * data.n * sizeof(int));

    int number_of_threads = 0;
    pthread_t *thread;
    number_of_threads = l * n;
    thread = (pthread_t *)malloc(number_of_threads * sizeof(pthread_t));
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int *row_column = malloc(sizeof(*row_column) * 2);
            row_column[0] = i;
            row_column[1] = j;
            pthread_create(&thread[i], NULL, v1, row_column);
        }
    }

    for (int i = 0; i < number_of_threads; i++)
    {
        pthread_join(thread[i], NULL);
    }

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < n; j++)
        {
            Item(C, i, j, n) = Item(data.C, i, j, n);
        }
    }
}

/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult_v2(int *A, int *B, int *C, int l, int m, int n)
{
    data.l = l;
    data.m = m;
    data.n = n;
    data.A = A;
    data.B = B;
    data.C = (int *)malloc(data.l * data.n * sizeof(int));

    int number_of_threads = 0;
    pthread_t *thread;
    number_of_threads = l;
    thread = (pthread_t *)malloc(number_of_threads * sizeof(pthread_t));
    for (int i = 0; i < number_of_threads; i++)
    {
        int *row = malloc(sizeof(*row));
        *row = i;
        pthread_create(&thread[i], NULL, v2, row);
    }

    for (int i = 0; i < number_of_threads; i++)
    {
        pthread_join(thread[i], NULL);
    }

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < n; j++)
        {
            Item(C, i, j, n) = Item(data.C, i, j, n);
        }
    }
}
