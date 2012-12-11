#ifndef HILLCLIMBER_H
#define HILLCLIMBER_H

#include "algorithm.h"
#include "partition.h"
#include "rng.h"

//comment the following if random initial solution is not required
#define RND_INIT_HC

typedef unsigned int UINT;

class HillClimber : public Algorithm
{
    private:
		Partition 	*currPartition;
		double	currCost;
		double	minCost;
		long iterations;
		RNG rng;

    public:
		HillClimber();
		void InitialSelection();
		void Apply();
		void Step();
};

#endif
