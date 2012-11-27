#ifndef HILLCLIMBER_H
#define HILLCLIMBER_H

#include "algorithm.h"
#include "partition.h"
#include "rng.h"

typedef unsigned int UINT;

class HillClimber : public Algorithm
{
    private:
		Partition 	*currPartition;
		double	currCost;
		double	minCost;
		double	initTemp;
		double	decayRate;
		long iterations;
		RNG rng;

    public:
		HillClimber();
		void InitialSelection();
		void Apply();
		void Step();
};

#endif
