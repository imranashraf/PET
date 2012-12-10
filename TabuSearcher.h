#ifndef TABUSEARCHER_H
#define TABUSEARCHER_H

#include <vector>

#include "algorithm.h"
#include "partition.h"
#include "rng.h"

typedef unsigned int UINT;

#define TABU_SIZE 4
#define TABU_TENURE 3

class TabuList 
{
	private:
		UINT ***tList;
		UINT size_x, size_y, size_z;
		
	public: 
		TabuList(UINT nclusters, UINT nfunctions)
		{
			size_x = nclusters;
			size_y = nclusters;
			size_z = nfunctions;
			
			tList = new UINT **[size_x];
			for (UINT i = 0; i < size_x; ++i) 
			{
				tList[i] = new UINT* [size_y];
				for (UINT j = 0; j < size_y; ++j) 
				{
					tList[i][j] = new UINT[size_z];
				}
			}
		}
		
		void tabuMove(UINT srcCNo, UINT dstCNo, UINT fno)
		{ 
			tList[srcCNo][dstCNo][fno]+= TABU_TENURE;
		}

		UINT getTabuValue(UINT srcCNo, UINT dstCNo, UINT fno)
		{ 
			return tList[srcCNo][dstCNo][fno];
		}

		void decrementTabu()
		{
			for(UINT i = 0; i<size_x; i++)
			{
				for(UINT j = 0; j<size_y; j++)
				{
					for(UINT k = 0; k<size_z; k++)
					{
						if( tList[i][j][k] >= 1)
							tList[i][j][k]--;
					}
				} 
			}
		}
		
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
