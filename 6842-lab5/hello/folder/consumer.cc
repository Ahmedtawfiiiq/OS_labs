#include <unistd.h>
#include <semaphore.h>
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

#define SEM_MUTEX_KEY 10
#define SEM_BUFFER_COUNT_KEY 20
#define SEM_SPOOL_SIGNAL_KEY 30

#define BUFFERSIZE 10

using namespace std;

sem_t semEmpty;
sem_t semFull;
pthread_mutex_t mutexBuffer;

vector<float> buffer(BUFFERSIZE, 0.0);
int count = 0;

float priceGenerator(float mean, float standard_deviation)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<float> distribution(mean, standard_deviation);

    return distribution(generator);
}

void producer()
{
    while (true)
    {
        // Produce
        float item = priceGenerator(30, 0.04);

        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        // critical section
        buffer[count] = item;
        count++;
        cout << "produced item: " << item << endl;
        // end of critical section

        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);

        // sleep(1);
    }
}

void consumer()
{
    while (true)
    {
        float item;

        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);

        // critical section
        item = buffer[count - 1];
        count--;
        // end of critical section

        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);

        // Consume
        cout << "consumed item:  " << item << endl;
        // sleep(1);
    }
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, BUFFERSIZE);
    sem_init(&semFull, 0, 0);

    union semun
    {
        int val;
        struct semid_ds *buf;
        ushort array[1];
    } sem_attr;

    // threads creation

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}