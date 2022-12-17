#include "header.hpp"

int mutex_sem, empty_sem, full_sem;
int shm_id, c_id;

unordered_map<string, int> umap1;
unordered_map<int, string> umap2;

typedef struct commodities
{
    int number = 0;
    float price[READINGS]; // current price and previous 4 prices
    float average[2];
} commodity;

commodity *p;
int *count;

float priceGenerator(float mean, float standard_deviation)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    normal_distribution<float> distribution(mean, standard_deviation);

    return distribution(generator);
}

void modifyPrice(commodity *c, int *i, float item)
{
    switch (*i)
    {
    case 0:
        c->price[*i] = item;
        break;

    case 1:
        c->price[*i] = c->price[*i - 1];
        c->price[*i - 1] = item;
        break;
    case 2:
        c->price[*i] = c->price[*i - 1];
        c->price[*i - 1] = c->price[*i - 2];
        c->price[*i - 2] = item;
        break;
    case 3:
        c->price[*i] = c->price[*i - 1];
        c->price[*i - 1] = c->price[*i - 2];
        c->price[*i - 2] = c->price[*i - 3];
        c->price[*i - 3] = item;
        break;
    case 4:
        c->price[*i] = c->price[*i - 1];
        c->price[*i - 1] = c->price[*i - 2];
        c->price[*i - 2] = c->price[*i - 3];
        c->price[*i - 3] = c->price[*i - 4];
        c->price[*i - 4] = item;
        break;
    case 5:
        c->price[*i - 1] = c->price[*i - 2];
        c->price[*i - 2] = c->price[*i - 3];
        c->price[*i - 3] = c->price[*i - 4];
        c->price[*i - 4] = c->price[*i - 5];
        c->price[*i - 5] = item;
        *i -= 1;
        break;
    }

    *i += 1;
}

void modifyAverage(commodity *c, int *j, float average)
{
    switch (*j)
    {
    case 0:
        c->average[*j] = average;
        break;

    case 1:
        c->average[*j] = c->average[*j - 1];
        c->average[*j - 1] = average;
        *j -= 1;
        break;
    }

    *j += 1;
}

float calculateAverage(commodity *c)
{
    float sum = 0.0;
    int i = 0;
    while (c->price[i] != 0 && i < READINGS)
    {
        sum += c->price[i];
        i++;
    }

    return sum / i;
}

void producer(int commodity_number, float mean, float standard_deviation, float sleep_interval)
{
    struct sembuf asem[1];
    asem[0].sem_num = 0;
    asem[0].sem_op = 0;
    asem[0].sem_flg = 0;

    commodity c;
    for (int i = 0; i < READINGS; i++)
    {
        c.price[i] = 0.0;
    }
    int i = 0;
    int j = 0;

    while (true)
    {
        // produce
        float item = priceGenerator(mean, standard_deviation);
        fprintf(stderr, "%s: generating a new value %f\n", umap2[commodity_number].c_str(), item);

        asem[0].sem_op = -1;
        if (semop(empty_sem, asem, 1) == -1)
        {
            perror("semop: empty_sem");
            exit(1);
        }

        fprintf(stderr, "%s: trying to get mutex on shared buffer\n", umap2[commodity_number].c_str());

        asem[0].sem_op = -1;
        if (semop(mutex_sem, asem, 1) == -1)
        {
            perror("semop: mutex_sem");
            exit(1);
        }

        // critical section

        fprintf(stderr, "%s: placing %f on shared buffer\n", umap2[commodity_number].c_str(), item);

        p = (commodity *)shmat(shm_id, NULL, 0);
        count = (int *)shmat(c_id, NULL, 0);

        // printf("\nitem count -> %d", count[0]);

        c.number = commodity_number;
        modifyPrice(&c, &i, item);
        modifyAverage(&c, &j, calculateAverage(&c));

        // printf("\n");
        // for (int i = 0; i < READINGS; i++)
        // {
        //     printf("%f ", c.price[i]);
        // }

        // printf("\n");
        // for (int i = 0; i < 2; i++)
        // {
        //     printf("%f ", c.average[i]);
        // }

        p[count[0]] = c;
        count[0]++;

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
        if (semop(full_sem, asem, 1) == -1)
        {
            perror("semop: full_sem");
            exit(1);
        }

        fprintf(stderr, "%s: sleeping for %f ms\n", umap2[commodity_number].c_str(), sleep_interval);
        sleep(sleep_interval/1000);
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

    // empty semaphore
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

    umap1["ALUMINIUM"] = 0;
    umap1["COPPER"] = 1;
    umap1["COTTON"] = 2;
    umap1["CRUDEOIL"] = 3;
    umap1["GOLD"] = 4;
    umap1["LEAD"] = 5;
    umap1["MENTHAOIL"] = 6;
    umap1["NATURALGAS"] = 7;
    umap1["NICKEL"] = 8;
    umap1["SILVER"] = 9;
    umap1["ZINC"] = 10;

    umap2[0] = "ALUMINIUM";
    umap2[1] = "COPPER";
    umap2[2] = "COTTON";
    umap2[3] = "CRUDEOIL";
    umap2[4] = "GOLD";
    umap2[5] = "LEAD";
    umap2[6] = "MENTHAOIL";
    umap2[7] = "NATURALGAS";
    umap2[8] = "NICKEL";
    umap2[9] = "SILVER";
    umap2[10] = "ZINC";

    // argv[0] // program name
    // argv[1] // commodity name
    // argv[2] // commodity price mean
    // argv[3] // commodity price standard deviation
    // argv[4] // sleep interval
    // argv[5] // buffer size

    string commodity_name;
    commodity_name = argv[1];

    int commodity_number = umap1[commodity_name];

    float mean;
    mean = atof(argv[2]);

    float standard_deviation;
    standard_deviation = atof(argv[3]);

    float sleep_interval;
    sleep_interval = atof(argv[4]);

    producer(commodity_number, mean, standard_deviation, sleep_interval);

    return 0;
}