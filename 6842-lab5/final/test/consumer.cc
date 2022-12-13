#include "header.hpp"

extern int shmID;
extern struct product *p;
extern int mutex_sem, empty_sem, full_sem;
extern int front, rear;

float deQueue()
{
    float item;
    p = (struct product *)shmat(shmID, NULL, 0);

    if (front == -1)
        printf("\nqueue is empty");
    else
    {
        item = p[front].item_value;
        front++;
        if (front > rear)
            front = rear = -1;
    }

    shmdt(p);
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