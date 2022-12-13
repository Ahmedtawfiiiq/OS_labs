#include "queue.hpp"

void enQueue(float value)
{
    p = (struct product *)shmat(shmID, NULL, 0);

    if (rear == MEMORY_SIZE - 1)
        printf("\nqueue is full");
    else
    {
        if (front == -1)
            front = 0;
        rear++;
        p[rear].item_value = value;
        printf("\nproduced item -> %f", value);
    }

    shmdt(p);
}

float deQueue()
{
    float item;
    p = (struct product *)shmat(shmID, NULL, 0);

    if (front == -1)
        printf("\nqueue is empty");
    else
    {
        item = p[front].item_value;
        front++;
        if (front > rear)
            front = rear = -1;
    }

    shmdt(p);
    return item;
}
