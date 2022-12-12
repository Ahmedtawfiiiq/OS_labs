#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <random>
#include <chrono>
#include <vector>

#define SEM_MUTEX_KEY "sem-mutex-key"
#define SEM_BUFFER_COUNT_KEY "sem-buffer-count-key"
#define SEM_SPOOL_SIGNAL_KEY "sem-spool-signal-key"

#define THREAD_NUM 21

using namespace std;

struct product
{
    char name[30];
    float mean;
    float standard_deviation;
    float item_value;
};

#define MEMORY_SIZE sizeof(struct product) * 30

int shmID;
struct product *p;

int mutex_sem, empty_sem, full_sem;

float priceGenerator(float mean, float standard_deviation)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<float> distribution(mean, standard_deviation);

    return distribution(generator);
}

int front = -1, rear = -1;

void enQueue(float value)
{
    p = (struct product *)shmat(shmID, NULL, 0);

    if (rear == MEMORY_SIZE - 1)
        printf("\nqueue is full");
    else
    {
        if (front == -1)
            front = 0;
        rear++;
        p[rear].item_value = value;
        printf("\nproduced item -> %f", value);
    }

    shmdt(p);
}

float deQueue()
{
    float item;
    p = (struct product *)shmat(shmID, NULL, 0);

    if (front == -1)
        printf("\nqueue is empty");
    else
    {
        item = p[front].item_value;
        front++;
        if (front > rear)
            front = rear = -1;
    }

    shmdt(p);
    return item;
}

void *producer(void *args)
{
    struct sembuf asem[1];
    asem[0].sem_num = 0;
    asem[0].sem_op = 0;
    asem[0].sem_flg = 0;

    while (true)
    {
        // produce
        float item = priceGenerator(30, 0.04);

        asem[0].sem_op = -1;
        if (semop(empty_sem, asem, 1) == -1)
        {
            perror("semop: empty_sem");
            exit(1);
        }

        asem[0].sem_op = -1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        // critical section
        enQueue(item);
        // end of critical section

        asem[0].sem_op = 1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        asem[0].sem_op = 1;
        if (semop(full_sem, asem, 1) == -1)
        {
            perror("semop: full_sem");
            exit(1);
        }

        sleep(1);
    }
}

void *consumer(void *args)
{
    struct sembuf asem[1];
    asem[0].sem_num = 0;
    asem[0].sem_op = 0;
    asem[0].sem_flg = 0;

    while (true)
    {
        float item;

        asem[0].sem_op = -1;
        if (semop(full_sem, asem, 1) == -1)
        {
            perror("semop: full_sem");
            exit(1);
        }

        asem[0].sem_op = -1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        // critical section
        item = deQueue();
        // end of critical section

        asem[0].sem_op = 1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        asem[0].sem_op = 1;
        if (semop(empty_sem, asem, 1) == -1)
        {
            perror("semop: empty_sem");
            exit(1);
        }

        // consume
        printf("\nconsumed item -> %f", item);
        // sleep(1);
    }
}

int main()
{
    /* for semaphore */
    key_t s_key;
    union semun
    {
        int val;
        struct semid_ds *buf;
        ushort array[1];
    } sem_attr;

    // mutex semaphore
    if ((s_key = ftok(SEM_MUTEX_KEY, 'a')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((mutex_sem = semget(s_key, 1, 0660 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }
    // Giving initial value.
    sem_attr.val = 1; // unlocked
    if (semctl(mutex_sem, 0, SETVAL, sem_attr) == -1)
    {
        perror("semctl SETVAL");
        exit(1);
    }

    // empty semaphore
    if ((s_key = ftok(SEM_BUFFER_COUNT_KEY, 'a')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((empty_sem = semget(s_key, 1, 0660 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }
    // giving initial values
    sem_attr.val = MEMORY_SIZE;
    if (semctl(empty_sem, 0, SETVAL, sem_attr) == -1)
    {
        perror(" semctl SETVAL ");
        exit(1);
    }

    // full semaphore
    if ((s_key = ftok(SEM_SPOOL_SIGNAL_KEY, 'a')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((full_sem = semget(s_key, 1, 0660 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }
    // giving initial values
    sem_attr.val = 0; // 0 number of initial products
    if (semctl(full_sem, 0, SETVAL, sem_attr) == -1)
    {
        perror(" semctl SETVAL ");
        exit(1);
    }

    // 1000 -> key
    // sizeof(struct product) * 30 -> size
    shmID = shmget(1000, sizeof(struct product) * 30, 0666 | IPC_CREAT);
    printf("shmID = %d\n", shmID);
    if (shmID < 0)
    {
        printf("failed to create shm\n");
        exit(1);
    }

    pthread_t th[THREAD_NUM];

    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (i > 0)
        {
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0)
            {
                perror("Failed to create thread");
            }
        }
        else
        {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0)
            {
                perror("Failed to create thread");
            }
        }
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }

    // remove semaphores
    if (semctl(mutex_sem, 0, IPC_RMID) == -1)
    {
        perror("semctl IPC_RMID");
        exit(1);
    }
    if (semctl(empty_sem, 0, IPC_RMID) == -1)
    {
        perror("semctl IPC_RMID");
        exit(1);
    }
    if (semctl(full_sem, 0, IPC_RMID) == -1)
    {
        perror("semctl IPC_RMID");
        exit(1);
    }

    return 0;
}