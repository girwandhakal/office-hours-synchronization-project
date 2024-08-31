#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mytime.h"
 
int mytime (int left, int right)
{
	int random_time = 0;
	srand(time(NULL));
	random_time = left + rand()%(right - left);	
	//printf("random time is %d sec\n", random_time);
	return random_time;
}