#include <stdio.h>
#include <iostream>
#include <string.h>

#define SIZE 5

typedef struct commodities
{
    float price[5];
} commodity;

using namespace std;

void calculateAverage(commodity *c)
{
    float average = 0.0;
    for (int i = 0; i < 5 && c->price[i] != 0; i++)
    {
        
    }
}

int main(int argc, char *argv[])
{
    commodity c;
    c.price[0] = 12.6;
    c.price[1] = 3.1;

    calculateAverage(&c);

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