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

