#ifndef _ALGORITHM_H
#define _ALGORITHM_H

#include "partition.h"
#include "application.h"

class Algorithm
{
	public:
		Algorithm(){}
		
		virtual void Apply(unsigned int k)=0;
	
};

#endif
