#ifndef TABUSEARCHER_H
#define TABUSEARCHER_H

#include <vector>

#include "algorithm.h"
#include "partition.h"
#include "rng.h"

typedef unsigned int UINT;

#define TABU_SIZE 4
#define TABU_TENURE 9

//comment the following if random initial solution is not required
#define RND_INIT_TS

class TabuList 
{
	private:
		UINT ***tList;
		UINT size_x, size_y, size_z;
		
	public: 
		TabuList(UINT nclusters, UINT nfunctions);
		void tabuMove(UINT srcCNo, UINT dstCNo, UINT fno);
		UINT getTabuValue(UINT srcCNo, UINT dstCNo, UINT fno);
		void decrementTabu();
		void printTabu();
		
		~TabuList()
		{
			for (UINT i = 0; i < size_x; ++i) 
			{
				for (UINT j = 0; j < size_y; ++j) 
				{
					delete [] tList[i][j];
				}
				delete [] tList[i];
			}
			
			delete [] tList;
			tList = NULL;			
		}
};

class TabuSearcher : public Algorithm
{
    private:
		Partition* currPartition;
		TabuList * tabuList;
		double	currCost;
		double	minCost;
		long iterations;
		RNG rng;

    public:
		TabuSearcher();
		void InitialSelection();
		void Apply();
		Partition getBestNeighbour();
		
		~TabuSearcher()
		{
			delete currPartition;
			delete tabuList;
		}
};

#endif
