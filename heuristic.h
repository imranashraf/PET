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
		Heuristic();
		void Apply();
		void InitialSelection();
// 		void InitialSelection(Partition & heurPartition);
};

#endif

