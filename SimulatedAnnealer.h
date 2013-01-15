#ifndef SIMULATEDANNEALER_H
#define SIMULATEDANNEALER_H

#include "algorithm.h"
#include "partition.h"
#include "rng.h"

typedef unsigned int UINT;

class SimulatedAnnealer : public Algorithm
{
    private:
		Partition currPartition;
		double	currCost;
		double	minCost;
		double	initTemp;
		double	decayRate;
		long iterations;
		RNG rng;

    public:
		SimulatedAnnealer();
		void InitialSelection();
		void Apply();
		void Step();
};

#endif
