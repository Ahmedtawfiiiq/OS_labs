#include "header.hpp"

int front = -1, rear = -1;

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