#ifndef UTILITY_H
#define UTILITY_H

#include <sys/time.h>
 
class Timer
{
	private:
		timeval _StartTime;
		timeval _StopTime;
		
	public:
		Timer(){}
		void Start();
		void Stop();
		void Print();
};

void bsort(double Array[], int n);
int Max(double Array[], int n);
std::string EstimateTime(long long partitions);
std::string PrintTime(long long partitions);

#endif 
