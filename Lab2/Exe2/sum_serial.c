#include <stdio.h>
#include <stdlib.h>
void serial(int num)
{
	int n, sum= 0;
	n= num;
	for (int i= 1; i <= n; i++)
	{
		sum+= i;
	}
	printf("Result of sum_serial: %d\n", sum);
} 
