#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct args
{
    char *name;
    int age;
};

void *hello(void *input)
{
    printf("name: %s\n", ((struct args *)input)->name);
    printf("age: %d\n", ((struct args *)input)->age);
}

int main()
{
    struct args *Ahmed = (struct args *)malloc(sizeof(struct args));
    char ahmed[] = "ahmed";
    Ahmed->name = ahmed;
    Ahmed->age = 20;

    pthread_t tid;
    pthread_create(&tid, NULL, hello, (void *)Ahmed);
    pthread_join(tid, NULL);
    return 0;
}