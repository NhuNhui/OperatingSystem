#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
typedef struct
{
	int rating[1683];
	int freq[1683];
	float result[1683];
}movies;

void executeFile1()
{
	int first, second, third, four;
	key_t key= ftok("Lab2_Exe1.c", 'A');
	int shmid= shmget(key, sizeof(movies), 0666 | IPC_CREAT);
	movies* arr= (movies*)shmat(shmid, NULL, 0);
	FILE* fileObj= fopen("movie-100k_1.txt", "r");
	while (fscanf(fileObj, "%d %d %d %d", &first, &second, &third, &four)!= EOF)
	{
		arr->freq[second]++;
		arr->rating[second]+= third;
	}
	shmdt(arr);
}

void executeFile2()
{
	int first, second, third, four;
	key_t key= ftok("Lab2_Exe1.c", 'A');
	int shmid= shmget(key, sizeof(movies), 0666 | IPC_CREAT);
	movies* arr= (movies*)shmat(shmid, NULL, 0);
	FILE* fileObj= fopen("movie-100k_2.txt", "r");
	while (fscanf(fileObj, "%d %d %d %d", &first, &second, &third, &four)!= EOF)
	{
		arr->freq[second]++;
		arr->rating[second]+= third;
	}
	shmdt(arr);
}

void calcAverage()
{
	key_t key= ftok("Lab2_Exe1.c", 'A');
	int shmid= shmget(key, sizeof(movies), 0666 | IPC_CREAT);
	movies* arr= (movies*)shmat(shmid, NULL, 0);
	for (int i= 1; i < 1682; i++)
	{
		arr->result[i]= arr->rating[i]*1.0/ arr->freq[i];
	}
	int choose;
	//printf("Average rating of moviesID_1: %f\n", arr->result[1]);
	printf("Enter movies_ID you want to calc average: ");
	scanf("%d", &choose);
	printf("Average rating of moviesID_%d: %f\n", choose,  arr->result[choose]);
	shmdt(arr);
	shmctl(shmid, IPC_RMID, 0);
}

int main()
{
	// create the key
	key_t key= ftok("Lab2_Exe1.c", 'A');
	
	// create the shared memory
	int shmid= shmget(key, sizeof(movies), 0666 | IPC_CREAT);
	
	//attach
	movies* arr= (movies*)shmat(shmid, NULL, 0);
	for (int i= 0; i < 1683; i++)
	{
		arr->rating[i]= 0;
		arr->freq[i]= 0;
		arr->result[i]= 0;
	}
	
	// dettach
	shmdt(arr);
	
	int pid= fork();
	if (pid != 0)
	{
		wait(NULL);
		int pid_2= fork();
		if (pid_2== 0)
		{
			executeFile2();
		}
		else
		{
			wait(NULL);
			calcAverage();
		}
		
	}
	if (pid == 0)
	{
		executeFile1();
	}
			
	return 0;
}
