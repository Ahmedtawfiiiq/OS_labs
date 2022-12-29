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

void populateMatrix(int *X, int nrow, int ncol)
{
    for (int i = 0; i < nrow; i++)
        for (int j = 0; j < ncol; j++)
            Item(X, i, j, ncol) = (((long)X) + ((long)&nrow) + ((long)&nrow) - (~(i * ncol + nrow) + (j * nrow + ncol))) % 29311 % 10;
}

void printMatrix(int *X, int nrow, int ncol)
{
    for (int i = 0; i < nrow; i++)
    {
        for (int j = 0; j < ncol; j++)
            printf("%d\t", Item(X, i, j, ncol));
        printf("\n");
    }
    printf("\n");
    printf("\n");
}

void *v2(void *row)
{
    int currentRow = *((int *)row);
    printf("current row -> %d\n", currentRow);
    for (int j = 0; j < data.n; j++)
    {
        int sum = 0;
        for (int k = 0; k < data.m; k++)
            sum += Item(data.A, currentRow, k, data.m) * Item(data.B, k, j, data.n);
        Item(data.C, currentRow, j, data.n) = sum;
    }

    for (int j = 0; j < data.n; j++)
        printf("%d\t", Item(data.C, currentRow, j, data.n));
    printf("\n");
}

void *v1(void *row_column)
{
    int currentRow = ((int *)row_column)[0];
    int currentColumn = ((int *)row_column)[1];

    printf("current row -> %d\n", currentRow);
    printf("current column -> %d\n", currentColumn);

    int sum = 0;
    for (int k = 0; k < data.m; k++)
        sum += Item(data.A, currentRow, k, data.m) * Item(data.B, k, currentColumn, data.n);
    Item(data.C, currentRow, currentColumn, data.n) = sum;

    printf("%d\t", Item(data.C, currentRow, currentColumn, data.n));
    printf("\n");
}

int main()
{
    data.l = 3;
    data.m = 4;
    data.n = 5;
    data.A = (int *)malloc(data.l * data.m * sizeof(int));
    data.B = (int *)malloc(data.m * data.n * sizeof(int));
    data.C = (int *)malloc(data.l * data.n * sizeof(int));

    populateMatrix(data.A, data.l, data.m);
    populateMatrix(data.B, data.m, data.n);

    printMatrix(data.A, data.l, data.m);
    printMatrix(data.B, data.m, data.n);

    int version = 1; // 0 for version 1 and 1 for version 2
    int number_of_threads = 0;
    pthread_t *thread;
    switch (version)
    {
    case 0:
        number_of_threads = data.l;
        thread = (pthread_t *)malloc(number_of_threads * sizeof(pthread_t));
        for (int i = 0; i < number_of_threads; i++)
        {
            int *row = malloc(sizeof(*row));
            *row = i;
            pthread_create(&thread[i], NULL, v2, row);
        }
        break;
    case 1:
        number_of_threads = data.l * data.n;
        thread = (pthread_t *)malloc(number_of_threads * sizeof(pthread_t));
        for (int i = 0; i < data.l; i++)
        {
            for (int j = 0; j < data.n; j++)
            {
                int *row_column = malloc(sizeof(*row_column) * 2);
                row_column[0] = i;
                row_column[1] = j;
                pthread_create(&thread[i], NULL, v1, row_column);
            }
        }
        break;
    }

    for (int i = 0; i < number_of_threads; i++)
    {
        pthread_join(thread[i], NULL);
    }

    printf("\n\n");
    printMatrix(data.C, data.l, data.n);

    free(data.A);
    free(data.B);
    free(data.C);

    return 0;
}
