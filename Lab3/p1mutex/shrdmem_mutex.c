#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock;

int MAX_COUNT = 1e9;
static int count = 0;

void *f_count(void *sid)
{
    pthread_mutex_lock(&lock);
    int i;
    for (i = 0; i < MAX_COUNT; i++)
    {
        count = count + 1;
    }
    printf("Thread %s: holding %d \n", (char *)sid, count);
    pthread_mutex_unlock(&lock);
}

int main()
{
    pthread_t thread1, thread2;

    pthread_mutex_init(&lock, NULL);

    /*Create independent threads*/
    pthread_create(&thread1, NULL, &f_count, "1");
    pthread_create(&thread2, NULL, &f_count, "2");

    // Wait for thread 1 finish
    pthread_join(thread1, NULL);

    // Wait for thread 2 finish
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}
