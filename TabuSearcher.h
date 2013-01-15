#ifndef TABUSEARCHER_H
#define TABUSEARCHER_H

#include <vector>

#include "algorithm.h"
#include "partition.h"
#include "rng.h"

typedef unsigned int UINT;

#define TABU_SIZE 5
#define TABU_TENURE 3

typedef std::vector< std::vector< std::vector< UINT > > > array3d;

class TabuList 
{
	private:
		array3d tList;
		UINT size_x, size_y, size_z;
		
	public: 
		TabuList()
		{
			size_x = 0;
			size_y = 0;
			size_z = 0;
		}
		
		TabuList(UINT nclusters, UINT nfunctions);
		void setTabuList(UINT nclusters, UINT nfunctions);
		void tabuMove(UINT srcCNo, UINT dstCNo, UINT fno);
		UINT getTabuValue(UINT srcCNo, UINT dstCNo, UINT fno);
		void decrementTabu();
		void printTabu(std::ostream & );
};

class TabuSearcher : public Algorithm
{
    private:
		Partition currPartition;
		TabuList tabuList;
		double	currCost;
		double	minCost;
		long iterations;
		RNG rng;

    public:
		TabuSearcher();
		void InitialSelection();
		void Apply();
		Partition getBestNeighbour();
};

#endif
