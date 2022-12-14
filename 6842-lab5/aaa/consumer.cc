#include "header.hpp"

int mutex_sem, empty_sem, full_sem;
int shm_id, c_id;
float *p;
int *count;

void consumer()
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
        p = (float *)shmat(shm_id, NULL, 0);
        count = (int *)shmat(c_id, NULL, 0);
        printf("\nitem count -> %d", count[0]);
        item = p[count[0] - 1];
        count[0]--;
        shmdt(p);
        shmdt(count);
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
    union semun
    {
        int val;
        struct semid_ds *buf;
        ushort array[1];
    } sem_attr;

    // mutex semaphore
    if ((mutex_sem = semget(MUTEX_SEM_KEY, 1, 0660 | IPC_CREAT)) == -1)
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
    if ((empty_sem = semget(EMPTY_SEM_KEY, 1, 0660 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }
    // giving initial values
    sem_attr.val = MEMORY_ITEMS;
    if (semctl(empty_sem, 0, SETVAL, sem_attr) == -1)
    {
        perror(" semctl SETVAL ");
        exit(1);
    }

    // full semaphore
    if ((full_sem = semget(FULL_SEM_KEY, 1, 0660 | IPC_CREAT)) == -1)
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

    // shared memory for buffer
    shm_id = shmget(MEMORY_1_KEY, MEMORY_1_SIZE, PERMISSIONS_FLAG);

    // shared memory for count
    c_id = shmget(MEMORY_2_KEY, MEMORY_2_SIZE, PERMISSIONS_FLAG);

    count = (int *)shmat(c_id, NULL, 0);
    count[0] = 0;

    shmdt(count);

    consumer();

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