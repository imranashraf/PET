#ifndef _ALGORITHM_H
#define _ALGORITHM_H

#include "globals.h"
#include "partition.h"
#include "application.h"

class Algorithm
{
	public:
		Algorithm(){}
		
		virtual void Apply()=0;
	
};

#endif
