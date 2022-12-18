#include "header.hpp"

int mutex_sem, empty_sem, full_sem;
int shm_id, c_id;

unordered_map<int, string> umap;

commodity arr[COMMODITIES_NUMBER];

commodity *p;
int *count;

void consumer()
{
    struct sembuf asem[1];
    asem[0].sem_num = 0;
    asem[0].sem_op = 0;
    asem[0].sem_flg = 0;

    while (true)
    {
        // float item_price;
        int item_number;
        // float average;

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
        p = (commodity *)shmat(shm_id, NULL, 0);
        count = (int *)shmat(c_id, NULL, 0);

        // item_price = p[count[0] - 1].price[0];
        item_number = p[0].number;
        arr[item_number] = p[0];

        // remove first element from the buffer
        for (int i = 0; i < count[0] - 1; i++)
        {
            p[i] = p[i + 1];
        }

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
        p = (commodity *)shmat(shm_id, NULL, 0);

        printf("\e[1;1H\e[2J"); // clear the screen and place the cursor at the top of the screen
        printf("+-------------------------------------+\n");
        printf("| Currency      |  Price   | AvgPrice |\n");
        printf("+-------------------------------------+\n");
        for (int i = 0; i < COMMODITIES_NUMBER; i++)
        {
            if (arr[i].price[0] > arr[i].price[1] && arr[i].average[0] > arr[i].average[1])
                printf("| %s| \033[1;32m%7.2f↑ \033[0m| \033[1;32m%7.2f↑ \033[0m|\n", umap[i].c_str(), arr[i].price[0], arr[i].average[0]);
            else if (arr[i].price[0] > arr[i].price[1] && arr[i].average[0] < arr[i].average[1])
                printf("| %s| \033[1;32m%7.2f↑ \033[0m| \033[1;31m%7.2f↓ \033[0m|\n", umap[i].c_str(), arr[i].price[0], arr[i].average[0]);
            else if (arr[i].price[0] < arr[i].price[1] && arr[i].average[0] > arr[i].average[1])
                printf("| %s| \033[1;31m%7.2f↓ \033[0m| \033[1;32m%7.2f↑ \033[0m|\n", umap[i].c_str(), arr[i].price[0], arr[i].average[0]);
            else if (arr[i].price[0] < arr[i].price[1] && arr[i].average[0] < arr[i].average[1])
                printf("| %s| \033[1;31m%7.2f↓ \033[0m| \033[1;31m%7.2f↓ \033[0m|\n", umap[i].c_str(), arr[i].price[0], arr[i].average[0]);
            else if (arr[i].price[0] > arr[i].price[1])
                printf("| %s| \033[1;32m%7.2f↑ \033[0m| \033[0m%7.2f  \033[0m|\n", umap[i].c_str(), arr[i].price[0], arr[i].average[0]);
            else if (arr[i].price[0] < arr[i].price[1])
                printf("| %s| \033[1;31m%7.2f↓ \033[0m| \033[0m%7.2f  \033[0m|\n", umap[i].c_str(), arr[i].price[0], arr[i].average[0]);
            else if (arr[i].average[0] > arr[i].average[1])
                printf("| %s| \033[0m%7.2f  | \033[1;32m%7.2f↑ \033[0m|\n", umap[i].c_str(), arr[i].price[0], arr[i].average[0]);
            else if (arr[i].average[0] < arr[i].average[1])
                printf("| %s| \033[0m%7.2f  | \033[1;31m%7.2f↓ \033[0m|\n", umap[i].c_str(), arr[i].price[0], arr[i].average[0]);
            else
                printf("| %s| \033[1;34m%7.2f  \033[0m| \033[1;34m%7.2f  \033[0m|\n", umap[i].c_str(), arr[i].price[0], arr[i].average[0]);
        }
        printf("+-------------------------------------+\n");

        shmdt(p);
    }
}

int main(int argc, char *argv[])
{
    // argv[0] // program name
    // argv[1] // buffer size

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
    sem_attr.val = atoi(argv[1]);
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
    // sizeof(commodities) * atoi(argv[5])
    int buffer_size = atoi(argv[1]);
    shm_id = shmget(MEMORY_1_KEY, sizeof(commodity), PERMISSIONS_FLAG);

    // shared memory for count
    c_id = shmget(MEMORY_2_KEY, sizeof(int), PERMISSIONS_FLAG);
    count = (int *)shmat(c_id, NULL, 0);
    count[0] = 0;
    shmdt(count);

    umap[0] = "ALUMINIUM     ";
    umap[1] = "COPPER        ";
    umap[2] = "COTTON        ";
    umap[3] = "CRUDEOIL      ";
    umap[4] = "GOLD          ";
    umap[5] = "LEAD          ";
    umap[6] = "MENTHAOIL     ";
    umap[7] = "NATURALGAS    ";
    umap[8] = "NICKEL        ";
    umap[9] = "SILVER        ";
    umap[10] = "ZINC          ";

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