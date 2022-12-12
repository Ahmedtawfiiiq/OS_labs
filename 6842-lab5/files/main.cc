#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <random>
#include <chrono>
#include <vector>

#define SEM_MUTEX_KEY "sem-mutex-key"
#define SEM_BUFFER_COUNT_KEY "sem-buffer-count-key"
#define SEM_SPOOL_SIGNAL_KEY "sem-spool-signal-key"

#define THREAD_NUM 21
#define BUFFER_SIZE 10

using namespace std;

vector<float> buffer(BUFFER_SIZE, 0.0);
int count = 0;

class product
{
public:
    string name;
    float mean;
    float standard_deviation;
};

class products
{
public:
    vector<product> items;
};

int mutex_sem, empty_sem, full_sem;
int item_counter = -1;

float priceGenerator(float mean, float standard_deviation)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<float> distribution(mean, standard_deviation);

    return distribution(generator);
}

product getProduct(int index)
{
    product p1;
    p1.name = "gas";
    p1.mean = 30;
    p1.standard_deviation = 0.3;

    product p2;
    p2.name = "oil";
    p2.mean = 12;
    p2.standard_deviation = 0.1;

    product p3;
    p3.name = "cars";
    p3.mean = 100;
    p3.standard_deviation = 0.01;

    product p4;
    p4.name = "clothes";
    p4.mean = 87;
    p4.standard_deviation = 0.7;

    product p5;
    p5.name = "food";
    p5.mean = 150;
    p5.standard_deviation = 1.9;

    products p;
    p.items.push_back(p1);
    p.items.push_back(p2);
    p.items.push_back(p3);
    p.items.push_back(p4);
    p.items.push_back(p5);

    return p.items[index];
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
        buffer[count] = item;
        count++;
        cout << "produced item: " << item << endl;

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
        item = buffer[count - 1];
        count--;

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
        cout << endl
             << "consumer item:  " << item << endl;
        sleep(1);
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
    sem_attr.val = BUFFER_SIZE; // buffer size
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