#ifndef _ALGORITHM_H
#define _ALGORITHM_H

#include "cluster.h"
#include "application.h"

class Algorithm
{
	public:
		Algorithm(){}
		
		virtual Cluster * Partition(Application applic , unsigned int k)=0;
	
};

#endif
