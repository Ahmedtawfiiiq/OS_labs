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

struct nota
{
    float n;
    char text[30];
};

int deleteElement(int id, int n, float x)
{
    struct nota *evernota = (struct nota *)shmat(id, NULL, 0);

    if (evernota[n - 1].n == x)
    {
        shmdt(evernota);
        return (n - 1);
    }

    // int prev = arr[n - 1], i;
    struct nota prev = evernota[n - 1];
    int i;
    for (i = n - 2; i >= 0 && evernota[i].n != x; i--)
    {
        struct nota curr = evernota[i];
        evernota[i] = prev;
        prev = curr;
    }

    if (i < 0)
    {
        shmdt(evernota);
        return 0;
    }

    evernota[i] = prev;
    shmdt(evernota);
    return (n - 1);
}

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

    printf("received value before: %f\n", evernota[0].n);

    int n = deleteElement(shmID, n, evernota[0].n);

    printf("received value after: %f\n", evernota[0].n);

    shmdt(evernota);

    return 0;
}
