#include "header.hpp"

int id;
int *v;

int shmID;
struct product *p;

int mutex_sem, empty_sem, full_sem;

int front, rear;

float deQueue()
{
    p = (struct product *)shmat(shmID, NULL, 0);

    v = (int *)shmat(id, NULL, 0);
    front = v[3];
    rear = v[4];

    float item;

    if (front == -1)
        printf("\nqueue is empty");
    else
    {
        item = p[front].item_value;
        front++;
        if (front > rear)
            front = rear = -1;
    }

    v[3] = front;
    v[4] = rear;

    shmdt(p);
    shmdt(v);
    return item;
}

void *consumer(void *args)
{
    struct sembuf asem[1];
    asem[0].sem_num = 0;
    asem[0].sem_op = 0;
    asem[0].sem_flg = 0;

    v = (int *)shmat(id, NULL, 0);

    while (true)
    {
        float item;

        full_sem = v[2];

        asem[0].sem_op = -1;
        if (semop(full_sem, asem, 1) == -1)
        {
            perror("semop: full_sem");
            exit(1);
        }

        v[2] = full_sem;

        mutex_sem = v[0];

        asem[0].sem_op = -1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        v[0] = mutex_sem;

        // critical section
        item = deQueue();
        // end of critical section

        mutex_sem = v[0];

        asem[0].sem_op = 1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        v[0] = mutex_sem;

        empty_sem = v[1];

        asem[0].sem_op = 1;
        if (semop(empty_sem, asem, 1) == -1)
        {
            perror("semop: empty_sem");
            exit(1);
        }

        v[1] = empty_sem;

        // consume
        printf("\nconsumed item -> %f", item);
        // sleep(1);
    }
}

int main()
{
    // shared memory
    shmID = shmget(1000, sizeof(struct product) * 30, 0666 | IPC_CREAT);
    if (shmID < 0)
    {
        printf("failed to create shm\n");
        exit(1);
    }

    // shared memory
    id = shmget(10, sizeof(int) * 5, 0666 | IPC_CREAT);
    if (id < 0)
    {
        printf("failed to create shm\n");
        exit(1);
    }

    // single thread for consumer
    pthread_t th[1];
    if (pthread_create(&th[1], NULL, &consumer, NULL) != 0)
    {
        perror("Failed to create thread");
    }
    if (pthread_join(th[1], NULL) != 0)
    {
        perror("Failed to join thread");
    }

    return 0;
}