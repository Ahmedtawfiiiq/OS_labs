#include "header.hpp"

int id;
int *v;

int shmID;
struct product *p;

int mutex_sem, empty_sem, full_sem;

int front, rear;

float priceGenerator(float mean, float standard_deviation)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<float> distribution(mean, standard_deviation);

    return distribution(generator);
}

void enQueue(float value)
{
    p = (struct product *)shmat(shmID, NULL, 0);

    v = (int *)shmat(id, NULL, 0);
    front = v[3];
    rear = v[4];

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

    v[3] = front;
    v[4] = rear;

    shmdt(p);
    shmdt(v);
}

void *producer(void *args)
{
    struct sembuf asem[1];
    asem[0].sem_num = 0;
    asem[0].sem_op = 0;
    asem[0].sem_flg = 0;

    v = (int *)shmat(id, NULL, 0);

    while (true)
    {
        // produce
        float item = priceGenerator(30, 0.04);

        empty_sem = v[1];

        asem[0].sem_op = -1;
        if (semop(empty_sem, asem, 1) == -1)
        {
            perror("semop: empty_sem");
            exit(1);
        }

        v[1] = empty_sem;

        mutex_sem = v[0];

        asem[0].sem_op = -1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        v[0] = mutex_sem;

        // critical section
        enQueue(item);
        // end of critical section

        mutex_sem = v[0];

        asem[0].sem_op = 1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        v[0] = mutex_sem;

        full_sem = v[2];

        asem[0].sem_op = 1;
        if (semop(full_sem, asem, 1) == -1)
        {
            perror("semop: full_sem");
            exit(1);
        }

        v[2] = full_sem;

        sleep(1);
    }

    shmdt(v);
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

    // single thread for producer
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