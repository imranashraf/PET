#ifndef _HEURISTIC_H
#define _HEURISTIC_H

#include <vector>
#include <set>

#include "partition.h"
#include "algorithm.h"
#include "cluster.h"

typedef unsigned int UINT;

class Heuristic : public Algorithm
{
	public:
		Heuristic(){}
		
		Heuristic(UINT k){ applic->CreateRanks(k);}
		
		void Apply(UINT k);
		
		void InitialSelection(Partition& partition, UINT k);
};

#endif

