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
#include <vector>
#include <unordered_map>

using namespace std;

#define MUTEX_SEM_KEY 10
#define EMPTY_SEM_KEY 20
#define FULL_SEM_KEY 30

#define MEMORY_1_KEY 40
#define MEMORY_1_SIZE sizeof(float) * 10

#define MEMORY_2_KEY 50
#define MEMORY_2_SIZE sizeof(int) * 1

#define PERMISSIONS_FLAG 0660 | IPC_CREAT

#define THREAD_NUM 21

#define MEMORY_ITEMS 10

#define READINGS 5
#define COMMODITIES_NUMBER 11