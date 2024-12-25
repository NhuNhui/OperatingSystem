#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_ITEMS 1
#define THREADS 1           // 1 producer and 1 consumer
#define LOOPS 2 * MAX_ITEMS // variable

sem_t sem_producer;
sem_t sem_consumer;

// Initiate shared buffer
int buffer[MAX_ITEMS];
int fill = 0;
int use = 0;

/* TODO: Fill in the sunchronization stuff */
void put(int value); // put data into buffer
int get();           // get data from buffer

void *producer(void *arg)
{
    int i = 0;
    int *tid = (int *)arg;
    for (i = 0; i < LOOPS; i++)
    {
        /*TODO: fill in the synchronization stuff */
        sem_wait(&sem_producer);
        put(i); // line P2
        printf("Producer %d put data %d\n", *tid, i);
        sleep(1);
        /*TODO: Fill in the synchronization stuff */
        sem_post(&sem_consumer);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    int i, tmp = 0;
    int *tid = (int *)arg;
    while (tmp != -1)
    {
        /*TODO: Fill in the synchronization stuff*/
        sem_wait(&sem_consumer);
        tmp = get();
        printf("Consumer %d get data %d\n", *tid, tmp);
        sleep(1);
        /*TODO: Fill in the synchronization stuff*/
        sem_post(&sem_producer);
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    int i, j;

    pthread_t producers[THREADS];
    pthread_t consumers[THREADS];

    /*TODO*/
    // producer ban dau co the vao MAX_ITEM lan
    // tu lan thu 2 phai doi consumer
    sem_init(&sem_producer, 0, MAX_ITEMS);
    // consumer phai doi producer put item roi signal moi chay
    sem_init(&sem_consumer, 0, 0);
    int tid[THREADS];
    for (i = 0; i < THREADS; i++)
    {
        tid[i] = i + 1;
        // Create producer thread
        pthread_create(&producers[i], NULL, producer, &tid[i]);
        // Create consumer thread
        pthread_create(&consumers[i], NULL, consumer, &tid[i]);
    }
    for (i = 0; i < THREADS; i++)
    {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    /*TODO*/
    sem_destroy(&sem_producer);
    sem_destroy(&sem_consumer);
    return 0;
}

void put(int value)
{
    buffer[fill] = value;
    fill = (fill + 1) % MAX_ITEMS;
}

int get()
{
    int tmp = buffer[use];
    use = (use + 1) % MAX_ITEMS;
    return tmp;
}
