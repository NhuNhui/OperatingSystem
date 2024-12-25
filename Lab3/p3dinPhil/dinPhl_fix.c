#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t sem[N];
// pthread_mutex_t lock;

void *philosopher(void *);
void eat(int);
void think(int);
int main()
{
    int i, a[N];
    pthread_t tid[N];

    /* BEGIN PROTECTION MECHANISM */
    // pthread_mutex_init(&lock, NULL);
    for (i = 0; i < N; i++)
    {
        sem_init(&sem[i], 0, 1);
    }
    /* END PROTECTION MECHANISM */

    for (i = 0; i < N; i++)
    {
        a[i] = i;
        pthread_create(&tid[i], NULL, philosopher, (void *)&a[i]);
    }
    for (i = 0; i < 5; i++)
        pthread_join(tid[i], NULL);

    // pthread_mutex_destroy(&lock);
    for (i = 0; i < N; i++)
    {
        sem_destroy(&sem[i]);
    }
}

void *philosopher(void *num)
{
    int phil = *(int *)num;
    int flag = 1;
    printf("Philosopher %d has entered room \n", phil);
    while (1)
    {
        // TODO
        /*PROTECTION*/
        // khong can mutex? lo 2 thang cung wait 1 cai thi sao
        sem_wait(&sem[phil]);
        sem_wait(&sem[(phil + 1) % N]);
        printf("Philosopher %d takes fork %d and %d\n", phil, phil, (phil + 1) % N);
        eat(phil);
        sleep(2);

        printf("Philosopher %d puts fork %d and %d down\n", phil, (phil + 1) % N, phil);
        /*PROTECTION*/
        sem_post(&sem[phil]);
        sem_post(&sem[(phil + 1) % N]);
        think(phil);
        sleep(1);
    }
}

void eat(int phil)
{
    printf("Philosopher %d is eating\n", phil);
}

void think(int phil)
{
    printf("Philosopher %d is thinking\n", phil);
}