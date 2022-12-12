#include <stdio.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

struct nota
{
    float n;
    char text[30];
};

int main()
{
    int shmID;
    struct nota *evernota;

    // 1000 -> key
    // sizeof(struct nota) * 30 -> size
    shmID = shmget(1000, sizeof(struct nota) * 30, 0666 | IPC_CREAT);
    printf("shmID = %d\n", shmID);
    if (shmID < 0)
    {
        printf("failed to create shm\n");
        exit(1);
    }

    evernota = (struct nota *)shmat(shmID, NULL, 0);

    evernota[0].n = 21.4;
    evernota[1].n = 47.8;

    printf("sent value: %f\n", evernota[0].n);
    printf("sent value: %f\n", evernota[1].n);

    shmdt(evernota);

    return 0;
}
