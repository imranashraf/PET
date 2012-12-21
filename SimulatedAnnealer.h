#ifndef SIMULATEDANNEALER_H
#define SIMULATEDANNEALER_H

#include "algorithm.h"
#include "partition.h"
#include "rng.h"

//comment the following if random initial solution is not required
#define RND_INIT_SA

typedef unsigned int UINT;

class SimulatedAnnealer : public Algorithm
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
		SimulatedAnnealer();
		void InitialSelection();
		void Apply();
		void Step();
		~SimulatedAnnealer()
		{
			delete currPartition;
			currPartition=NULL;
		}
};

#endif
