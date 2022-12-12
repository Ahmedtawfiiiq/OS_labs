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
#include <fstream>

using namespace std;

#define BUFFER_SIZE 10

int mutex_sem, empty_sem, full_sem;

vector<float> buffer(BUFFER_SIZE, 0.0);
int count = 0;

float priceGenerator(float mean, float standard_deviation)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<float> distribution(mean, standard_deviation);

    return distribution(generator);
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

        // empty semaphore
        asem[0].sem_op = -1;
        if (semop(empty_sem, asem, 1) == -1)
        {
            perror("semop: empty_sem");
            exit(1);
        }

        // mutex semaphore
        asem[0].sem_op = -1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        // critical section
        buffer[count] = item;
        count++;

        cout << endl;
        cout << "produced item: " << item;
        cout << endl;

        // mutex semaphore
        asem[0].sem_op = 1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        // full semaphore
        asem[0].sem_op = 1;
        if (semop(full_sem, asem, 1) == -1)
        {
            perror("semop: full_sem");
            exit(1);
        }

        sleep(1);
    }
}

int main()
{
    pthread_t th[1];

    if (pthread_create(&th[0], NULL, &producer, NULL) != 0)
    {
        perror("Failed to create thread");
    }

    if (pthread_join(th[0], NULL) != 0)
    {
        perror("Failed to join thread");
    }

    return 0;
}