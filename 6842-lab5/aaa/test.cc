#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define SIZE 5

typedef struct commodities
{
    float price[5];
    float average[2];
} commodity;

using namespace std;

float calculateAverage(commodity *c)
{
    float sum = 0.0;
    int i = 0;
    while (c->price[i] != 0 && i < 5)
    {
        sum += c->price[i];
        i++;
    }

    return sum / i;
}

void modify(commodity *c, int *i, float item)
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

int main(int argc, char *argv[])
{
    // fprintf(stderr, "error\n");
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    ios_base::sync_with_stdio(false);

    for (int i = 0; i < 1000000; i++)
    {
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken;
    time_taken = (end.tv_sec - start.tv_sec) * 1e9;
    time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;

    cout << time_taken << endl;

    // commodity c;

    // for (int i = 0; i < 5; i++)
    // {
    //     c.price[i] = 0;
    // }

    // for (int i = 0; i < 2; i++)
    // {
    //     c.average[i] = 0;
    // }

    // int i = 0;
    // int j = 0;

    // float item = 8.3;
    // float average;

    // while (true)
    // {
    //     modify(&c, &i, item);
    //     average = calculateAverage(&c);
    //     cout << "current average " << average << endl;
    //     modifyAverage(&c, &j, average);

    //     cout << "items" << endl;
    //     for (int i = 0; i < 5; i++)
    //     {
    //         cout << c.price[i] << " ";
    //     }
    //     cout << endl;

    //     cout << "average" << endl;
    //     for (int i = 0; i < 2; i++)
    //     {
    //         cout << c.average[i] << " ";
    //     }
    //     cout << endl;

    //     item += 6.1;
    //     sleep(1);
    // }

    // cout << calculateAverage(&c) << endl;

    // string name = argv[1];
    // cout << name << endl;
    // commodity p;
    // // strcpy(p.name, "gold");
    // // p.price = 1002.7;

    // commodity *ptr;
    // ptr = &p;

    // strcpy(ptr->name, "gold");
    // ptr->price = 1002.4;

    // printf("%f\n", ptr->price);

    // cout << p[0].name << endl;

    // int count = SIZE;

    // int arr[SIZE] = {1, 2, 3, 4, 5};

    // for (int i = 0; i < SIZE; i++)
    // {
    //     cout << arr[i] << " ";
    // }

    // cout << endl;
    // cout << "first item -> " << arr[0];
    // cout << endl;

    // count--;

    // for (int i = 0; i < count; i++)
    // {
    //     arr[i] = arr[i + 1];
    // }

    // for (int i = 0; i < count; i++)
    // {
    //     cout << arr[i] << " ";
    // }

    // cout << endl;
    // cout << "first item -> " << arr[0];
    // cout << endl;

    return 0;
}

// void initializeCommodities()
// {
//     p = (struct commodity *)shmat(shm_id, NULL, 0);

//     // commodity 1
//     strcpy(p[0].name, "GOLD");
//     p[0].price = 0.0;

//     // commodity 2
//     strcpy(p[1].name, "SILVER");
//     p[0].price = 0.0;

//     // commodity 3
//     strcpy(p[2].name, "CRUDEOIL");
//     p[0].price = 0.0;

//     // commodity 4
//     strcpy(p[3].name, "NATURALGAS");
//     p[0].price = 0.0;

//     // commodity 5
//     strcpy(p[4].name, "ALUMINIUM");
//     p[0].price = 0.0;

//     // commodity 6
//     strcpy(p[5].name, "COPPER");
//     p[0].price = 0.0;

//     // commodity 7
//     strcpy(p[6].name, "NICKEL");
//     p[0].price = 0.0;

//     // commodity 8
//     strcpy(p[7].name, "LEAD");
//     p[0].price = 0.0;

//     // commodity 9
//     strcpy(p[8].name, "ZINC");
//     p[0].price = 0.0;

//     // commodity 10
//     strcpy(p[9].name, "MENTHAOIL");
//     p[0].price = 0.0;

//     // commodity 11
//     strcpy(p[10].name, "COTTON");
//     p[0].price = 0.0;

//     shmdt(p);
// }