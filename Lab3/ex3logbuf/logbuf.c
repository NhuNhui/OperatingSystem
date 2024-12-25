#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_LOG_LENGTH 10
#define MAX_BUFFER_SLOT 6
#define MAX_LOOPS 30

char logbuf[MAX_BUFFER_SLOT][MAX_LOG_LENGTH];

pthread_mutex_t lock;         // lock cho ham wrlog
pthread_mutex_t flush_enable; // lock de biet khi nao can flush

int count;
void flushlog();

struct _args
{
    unsigned int interval;
};

void *wrlog(void *data)
{
    char str[MAX_LOG_LENGTH];
    int id = *(int *)data;

    usleep(20);

    // luu id vao str
    sprintf(str, "%d", id);
    // chuyen str vao logbuf[count]
    pthread_mutex_lock(&lock);
    strcpy(logbuf[count], str);
    count = (count > MAX_BUFFER_SLOT) ? count : (count + 1); /* Only increase count to size MAX_BUFFER_SLOT*/
    // printf("wrlog(): %d \n", id);

    if (count >= MAX_BUFFER_SLOT)
    {
        // buffer full ---> cho phep flush
        pthread_mutex_unlock(&flush_enable);
    }
    while (count >= MAX_BUFFER_SLOT)
    {
        // DO NOTHING
        // khong lam gi doi den khi flush xong
    }
    // unlock de cac wrlog khac co the thuc thi
    pthread_mutex_unlock(&lock);

    return 0;
}

void flushlog()
{
    int i;
    char nullval[MAX_LOG_LENGTH];

    // Doi tin hieu unlock tu wrlog
    pthread_mutex_lock(&flush_enable);

    // printf("flushlog()\n");
    sprintf(nullval, "%d", -1);

    for (i = 0; i < count; i++)
    {
        printf("Slot  %i: %s\n", i, logbuf[i]);
        strcpy(logbuf[i], nullval);
    }

    fflush(stdout);

    /*Reset buffer */
    count = 0;
    return;
}

void *timer_start(void *args)
{
    while (1)
    {
        flushlog();
        /*Waiting until the next timeout */
        usleep(((struct _args *)args)->interval);
    }
}

int main()
{
    int i;
    count = 0;
    pthread_t tid[MAX_LOOPS];
    pthread_t lgrid;
    int id[MAX_LOOPS];

    // khoi tao mutex
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&flush_enable, NULL);

    // Khoa khong cho flush
    pthread_mutex_lock(&flush_enable);

    struct _args args;
    args.interval = 500e3;
    /*500 msec ~ 500 * 1000 usec */

    /*Setup periodically invoke flushlog() */
    pthread_create(&lgrid, NULL, &timer_start, (void *)&args);

    /*Asynchronous invoke task writelog */
    for (i = 0; i < MAX_LOOPS; i++)
    {
        id[i] = i;
        pthread_create(&tid[i], NULL, wrlog, (void *)&id[i]);
    }
    // pthread_join(lgrid, NULL);
    for (i = 0; i < MAX_LOOPS; i++)
        pthread_join(tid[i], NULL);

    sleep(5);

    return 0;
}
