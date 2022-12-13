#include "producer.hpp"

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
