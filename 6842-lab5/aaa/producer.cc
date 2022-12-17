#include "header.hpp"

int mutex_sem, empty_sem, full_sem;
int shm_id, c_id, avg_id;

typedef struct commodities
{
    float price[READINGS]; // current price and previous 4 prices
} commodity;

commodity arr[COMMODITIES_NUMBER];

commodity *p;
int *count;
float *avg;

float priceGenerator(float mean, float standard_deviation)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<float> distribution(mean, standard_deviation);

    return distribution(generator);
}

float calculateAverage(commodity c)
{
    float average = 0.0;

    int i;
    for (i = 0; i < 5 && c.price[i] != 0; i++)
    {
        average += c.price[i];
    }
    average /= i;
    return average;
}

void producer(int commodity_number, float mean, float standard_deviation, float sleep_interval)
{
    struct sembuf asem[1];
    asem[0].sem_num = 0;
    asem[0].sem_op = 0;
    asem[0].sem_flg = 0;

    while (true)
    {
        // produce
        float item = priceGenerator(mean, standard_deviation);

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
        p = (commodity *)shmat(shm_id, NULL, 0);
        count = (int *)shmat(c_id, NULL, 0);
        avg = (float *)shmat(avg_id, NULL, 0);

        arr[commodity_number].price[0] = item;

        float average = calculateAverage(arr[commodity_number]);
        avg[commodity_number] = average;

        printf("\nitem count -> %d", count[0]);
        p[count[0]].price = item;
        count[0]++;
        shmdt(p);
        shmdt(count);
        printf("\nproduced item -> %f", item);
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

        sleep(sleep_interval);
    }
}

int main(int argc, char *argv[])
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

    // empty semaphoretypedef struct commodities
    {
        int number;
        float price = 0.0;
        float prevPrice[4];
        int prevPriceCount = 0;
    }
    commodity;

    if ((empty_sem = semget(EMPTY_SEM_KEY, 1, 0660 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }

    // full semaphore
    if ((full_sem = semget(FULL_SEM_KEY, 1, 0660 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }

    // shared memory for buffer
    shm_id = shmget(MEMORY_1_KEY, MEMORY_1_SIZE, PERMISSIONS_FLAG);

    // shared memory for count
    c_id = shmget(MEMORY_2_KEY, MEMORY_2_SIZE, PERMISSIONS_FLAG);

    // shared memory for average
    avg_id = shmget(MEMORY_3_KEY, MEMORY_3_SIZE, PERMISSIONS_FLAG);

    unordered_map<string, int> umap;
    umap["GOLD"] = 0;
    umap["SILVER"] = 1;
    umap["CRUDEOIL"] = 2;
    umap["NATURALGAS"] = 3;
    umap["ALUMINIUM"] = 4;
    umap["COPPER"] = 5;
    umap["NICKEL"] = 6;
    umap["LEAD"] = 7;
    umap["ZINC"] = 8;
    umap["MENTHAOIL"] = 9;
    umap["COTTON"] = 10;

    // argv[0] // program name
    // argv[1] // commodity name
    // argv[2] // commodity price mean
    // argv[3] // commodity price standard deviation
    // argv[4] // sleep interval
    // argv[5] // buffer size

    string commodity_name;
    commodity_name = argv[1];

    int commodity_number = umap[commodity_name];

    float mean;
    mean = atof(argv[2]);

    float standard_deviation;
    standard_deviation = atof(argv[3]);

    float sleep_interval;
    sleep_interval = atof(argv[4]);

    producer(commodity_number, mean, standard_deviation, sleep_interval);

    return 0;
}