#include <iostream>

#include "utility.h"


void Timer::Start()
{
	gettimeofday(&_StartTime, NULL);
}

void Timer::Stop()
{
	gettimeofday(&_StopTime,NULL);
}

void Timer::Print()
{
	double t1=_StartTime.tv_sec+(_StartTime.tv_usec/1000000.0);
	double t2=_StopTime.tv_sec+(_StopTime.tv_usec/1000000.0);
	
	std::cout<<"Time Elapsed = "<<t2-t1<<std::endl;
}

//sorting in descending order
//so Array[0] is the maximum
void bsort(double Array[], int n)
{
	long i, j;
	double temp;
	
	for (i = 0 ; i < ( n - 1 ); i++)
	{
		for (j = 0 ; j < n - i - 1; j++)
		{
			if (Array[j] < Array[j+1])
			{
				/* Swapping */
				
				temp         = Array[j];
				Array[j]   = Array[j+1];
				Array[j+1] = temp;
			}
		}
	}
}		

int Max(double Array[], int n)
{
	int i;
	int index=0;
	
	for(i=1;i<n;i++)
		if(Array[i] > Array[index] )
			index = i;
	
	return index;
}
