#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static int pipefd1[2], pipefd2[2];
char readMessage1[100];
char writeMessage1[100];
char readMessage2[100];
char writeMessage2[100];

void INIT(void)
{
	if (pipe(pipefd1) < 0 || pipe(pipefd2) < 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void WRITE_TO_PARENT(void)
{
	printf("Enter your string send to parrent: ");
	fgets(writeMessage1, 100, stdin);
	writeMessage1[strlen(writeMessage1)- 1]='\0';
	write(pipefd1[1], writeMessage1, sizeof(writeMessage1));
	printf("Child send message to parrent!\n");
}

void READ_FROM_PARENT(void)
{
	read(pipefd2[0], readMessage2, sizeof(readMessage2));
	printf("Child receive content: %s\n", readMessage2);
	printf("Child receive message from parrent!\n\n");
}

void WRITE_TO_CHILD(void)
{
	printf("Enter your string send to child: ");
	fgets(writeMessage2, 100, stdin);
	writeMessage2[strlen(writeMessage2)- 1]= '\0';
	write(pipefd2[1], writeMessage2, sizeof(writeMessage2));
	printf("Parent send message to child!\n");
}

void READ_FROM_CHILD(void)
{
	read(pipefd1[0], readMessage1, sizeof(readMessage1));
	printf("Parent receive content: %s\n", readMessage1);
	printf("Parent receive message from child!\n\n");
}

int main(int argc, char* argv[])
{
	INIT();
	pid_t pid;
	pid= fork();
	alarm(20);
	if (pid== 0)
	{
		while (1)
		{
			sleep(rand()%2 + 1);
			WRITE_TO_CHILD();
			READ_FROM_CHILD();
		}
	}
	else
	{
		while (1)
		{
			sleep(rand()%2 + 1);
			READ_FROM_PARENT();
			WRITE_TO_PARENT();
		}
	}
	return 0;
}
	
