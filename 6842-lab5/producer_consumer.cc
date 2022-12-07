#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// 5 producers
// 5 consumers
// using semaphore and mutex

#define MAX_ITEMS 5  // maximum items a producer can produce or a consumer can consume
#define BUFFERSIZE 5 // size of the buffer

sem_t empty;
sem_t full;

int in = 0;
int out = 0;
int buffer[BUFFERSIZE];
pthread_mutex_t mutex;

void *producer(void *pno)
{
    int item;
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        item = rand(); // produce a random item
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        printf("producer %d: insert item %d at %d\n", *((int *)pno), buffer[in], in);
        in = (in + 1) % BUFFERSIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    return NULL;
}

void *consumer(void *cno)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        printf("consumer %d: remove item %d from %d\n", *((int *)cno), item, out);
        out = (out + 1) % BUFFERSIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }

    return NULL;
}

int main()
{
    pthread_t pro[5], con[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFERSIZE);
    sem_init(&full, 0, 0);

    int a[5] = {1, 2, 3, 4, 5}; // numbering producer and consumer

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&pro[i], NULL, &producer, (void *)&a[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&con[i], NULL, &consumer, (void *)&a[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(pro[i], NULL);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}