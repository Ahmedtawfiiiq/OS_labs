#include "header.hpp"

int shmID, shmIDQ;
int *v;
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
    v = (int *)shmat(shmIDQ, NULL, 0);

    front = v[0];
    rear = v[1];

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

    v[0] = front;
    v[1] = rear;

    shmdt(p);
    shmdt(v);
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

int main()
{
    /* for semaphore */
    key_t s_key;

    // mutex semaphore
    if ((s_key = ftok(MUTEX_SEM_KEY, 'a')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((mutex_sem = semget(s_key, 1, 0660 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }

    // empty semaphore
    if ((s_key = ftok(EMPTY_SEM_KEY, 'a')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((empty_sem = semget(s_key, 1, 0660 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }

    // full semaphore
    if ((s_key = ftok(FULL_SEM_KEY, 'a')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((full_sem = semget(s_key, 1, 0660 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }

    shmID = shmget(1000, MEMORY_SIZE, 0666 | IPC_CREAT);
    if (shmID < 0)
    {
        printf("failed to create shm\n");
        exit(1);
    }

    shmIDQ = shmget(1001, sizeof(int) * 2, 0666 | IPC_CREAT);
    if (shmIDQ < 0)
    {
        printf("failed to create shm\n");
        exit(1);
    }

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