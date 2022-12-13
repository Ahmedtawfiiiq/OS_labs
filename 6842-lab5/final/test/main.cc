#include "header.hpp"

int shmID;
struct product *p;
int mutex_sem, empty_sem, full_sem;
int front = -1, rear = -1;

void *consumer(void *args);
void *producer(void *args);

int main()
{

    pthread_t th[1];

    if (pthread_create(&th[1], NULL, &producer, NULL) != 0)
    {
        perror("Failed to create thread");
    }

    if (pthread_join(th[1], NULL) != 0)
    {
        perror("Failed to join thread");
    }

    return 0;
}