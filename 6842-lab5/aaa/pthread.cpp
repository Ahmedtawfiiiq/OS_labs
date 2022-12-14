#include <unistd.h>
#include <semaphore.h>

#include <iostream>
#include <random>
#include <chrono>
#include <vector>

#define THREAD_NUM 21 // 20 thread for producers and 1 thread for consumer
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

void *producer(void *args)
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

void *consumer(void *args)
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
    pthread_t th[THREAD_NUM];

    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, BUFFERSIZE);
    sem_init(&semFull, 0, 0);

    int i;
    for (i = 0; i < THREAD_NUM; i++)
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
    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}