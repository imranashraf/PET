#ifndef _UTILITY_H
#define _UTILITY_H

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

#endif 
