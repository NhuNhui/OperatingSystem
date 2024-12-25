#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sum_serial.c"
int numThreads, number;

int result= 0;
void* calcSum(void* arg)
{

	int index = *(int*) arg;
	int start= ((index-1)*number/numThreads)+ 1;
	int end= (index*number/ numThreads);
	for (int i= start; i<= end; i++)
	{
		result+= i;
	}
	pthread_exit(NULL);
}


int main()
{	
	int* ptr= NULL;
	printf("Enter your number of threads: ");
	scanf("%d", &numThreads);
	printf("Enter your number to calc sum: ");
	scanf("%d", &number);
	pthread_t threads[numThreads];
	int threadID[numThreads];
	for (int a= 0; a < numThreads; a++)
	{	
		threadID[a]= a+ 1;
		pthread_create(&threads[a], NULL, calcSum, &threadID[a]);
	}
	for (int a= 0; a < numThreads; a++)
	{
		pthread_join(threads[a], NULL);
	}
	printf("Result of sum_multi-thread: %d\n", result);
	serial(number);
	return 0;
}
