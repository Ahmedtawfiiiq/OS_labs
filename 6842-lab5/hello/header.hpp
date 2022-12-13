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

#include <random>
#include <chrono>

using namespace std;

#define MUTEX_SEM_KEY "mutex-sem-key"
#define EMPTY_SEM_KEY "empty-sem-key"
#define FULL_SEM_KEY "full-sem-key"

#define MEMORY_SIZE sizeof(struct product) * 50

struct product
{
    char name[30];
    float mean;
    float standard_deviation;
    float item_value;
};
