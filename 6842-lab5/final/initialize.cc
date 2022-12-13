#include "header.hpp"

int mutex_sem, empty_sem, full_sem;

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

    // shared memory
    int *v;
    int id = shmget(10, sizeof(int) * 5, 0666 | IPC_CREAT);
    if (id < 0)
    {
        printf("failed to create shm\n");
        exit(1);
    }
    v = (int *)shmat(id, NULL, 0);
    v[0] = mutex_sem;
    v[1] = empty_sem;
    v[2] = full_sem;
    v[3] = front;
    v[4] = rear;

    return 0;
}