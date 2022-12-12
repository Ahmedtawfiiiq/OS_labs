#include <stdio.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

struct nota
{
    int n;
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

    printf("received value: %d\n", evernota[1].n);
    printf("received value: %s\n", evernota[1].text);

    shmdt(evernota);

    return 0;
}
