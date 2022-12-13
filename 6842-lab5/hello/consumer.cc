#include "header.hpp"

int shmID, shmIDQ;
int *v;
struct product *p;
int mutex_sem, empty_sem, full_sem;
int front, rear;

float deQueue()
{
    float item;
    p = (struct product *)shmat(shmID, NULL, 0);
    v = (int *)shmat(shmIDQ, NULL, 0);

    front = v[0];
    rear = v[1];

    if (front == -1)
        printf("\nqueue is empty");
    else
    {
        item = p[front].item_value;
        front++;
        if (front > rear)
            front = rear = -1;
    }

    v[0] = front;
    v[1] = rear;

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
    // Giving initial value.
    sem_attr.val = 1; // unlocked
    if (semctl(mutex_sem, 0, SETVAL, sem_attr) == -1)
    {
        perror("semctl SETVAL");
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
    // giving initial values
    sem_attr.val = MEMORY_SIZE;
    if (semctl(empty_sem, 0, SETVAL, sem_attr) == -1)
    {
        perror(" semctl SETVAL ");
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
    // giving initial values
    sem_attr.val = 0; // 0 number of initial products
    if (semctl(full_sem, 0, SETVAL, sem_attr) == -1)
    {
        perror(" semctl SETVAL ");
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

    v = (int *)shmat(shmIDQ, NULL, 0);

    v[0] = -1;
    v[1] = -1;

    shmdt(v);

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