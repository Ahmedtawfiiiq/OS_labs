#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>

#define SEM_MUTEX_KEY "sem-mutex-key"
#define SEM_BUFFER_COUNT_KEY "sem-buffer-count-key"
#define SEM_SPOOL_SIGNAL_KEY "sem-spool-signal-key"

#define THREAD_NUM 21

using namespace std;

struct product
{
    char name[30];
    float mean;
    float standard_deviation;
    float item_value;
};

#define MEMORY_SIZE sizeof(struct product) * 30

int shmID;
struct product *p;

int mutex_sem, empty_sem, full_sem;