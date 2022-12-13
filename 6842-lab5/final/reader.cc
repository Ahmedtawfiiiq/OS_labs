#include <stdio.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <vector>
#include <queue>

using namespace std;

// struct nota
// {
//     float n;
//     char text[30];
// };

int main()
{
    int shmID;
    // struct nota *evernota;
    int *value;

    // 1000 -> key
    // sizeof(struct nota) * 30 -> size
    shmID = shmget(1000, sizeof(int) * 5, 0666 | IPC_CREAT);
    printf("shmID = %d\n", shmID);
    if (shmID < 0)
    {
        printf("failed to create shm\n");
        exit(1);
    }

    value = (int *)shmat(shmID, NULL, 0);

    // printf("received value: %f\n", evernota[0].n);
    // printf("received value: %f\n", evernota[1].n);

    printf("%d\n", value[0]);
    printf("%d\n", value[1]);
    printf("%d\n", value[2]);
    printf("%d\n", value[3]);
    printf("%d\n", value[4]);

    shmdt(value);

    return 0;
}
