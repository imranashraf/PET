#include <iostream>
#include <cmath>

#include "TabuSearcher.h"
#include "globals.h"

using namespace std;

TabuList::TabuList(UINT nclusters, UINT nfunctions)
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
	
	for (UINT i = 0; i < size_x; ++i) 
	{
		for (UINT j = 0; j < size_y; ++j) 
		{
			for (UINT k = 0; k < size_z; ++k) 
			{
				tList[i][j][k] = 0;
			}
		}
	}
}

void TabuList::tabuMove(UINT srcCNo, UINT dstCNo, UINT fno)
{ 
	tList[srcCNo][dstCNo][fno]+= TABU_TENURE;
}

UINT TabuList::getTabuValue(UINT srcCNo, UINT dstCNo, UINT fno)
{ 
	return tList[srcCNo][dstCNo][fno];
}

void TabuList::decrementTabu()
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

void TabuList::printTabu(std::ostream & fout = std::cout)
{
	for(UINT i = 0; i<size_x; i++)
	{
		for(UINT j = 0; j<size_y; j++)
		{
			for(UINT k = 0; k<size_z; k++)
			{
				fout<<tList[i][j][k]<<"  ";
			}
			fout<<endl;
		} 
		fout<<endl;
	}
}

////////////////////////////////////////////

TabuSearcher::TabuSearcher()
{
	try
	{
		currPartition = new Partition(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	try
	{
		bestTSPartition = new Partition(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	//create tabu list
	try
	{
		tabuList = new TabuList(g_k, g_n);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	iterations = 100;
}

void TabuSearcher::InitialSelection()
{
	UINT fno, cno;

	#ifdef RND_INIT_TS
	for( fno=0; fno<g_n; fno++)
	#else	
	for( cno=0; cno<g_k; cno++)
	{
		fno = cno; 	//The functions are sorted in descending order
		currPartition->addFunction(fno,cno);
	}
	
	for( fno=g_k; fno<g_n; fno++)
	#endif
	{
		cno = 0 + ( abs( rng.rand_int31() ) % ( (g_k-1) - 0 + 1 ) );
		currPartition->addFunction(fno,cno);
	}
}

void TabuSearcher::Apply() 
{
	InitialSelection();
	
	*bestTSPartition = *currPartition;
	minCost= currPartition->Cost();
	
	dout<<"Initial minCost TS "<<minCost<<endl;

	for (int i = 0; i < iterations; i++) 
	{
		*currPartition = getBestNeighbour();
		currCost = currPartition->Cost();

		dout<<endl<<"currCost TS "<<currCost<<endl;
		dout<<"Current tabu list "<<endl;
		tabuList->printTabu(dout);
		
		if(currCost < minCost)
		{
			*bestTSPartition = *currPartition;
			minCost = currCost;
			dout<<endl<<"minCost TS "<<minCost<<endl;
		}
	}
}

Partition TabuSearcher::getBestNeighbour()
{
	Partition bestSol(g_n, g_k);
	double bestCost;
	
	Partition newBestSol(g_n, g_k);
	double newBestCost;
	
	UINT TotalClusters = g_k;
	UINT TotalFunctions;
	UINT srcCNo, dstCNo, fno;
	UINT tsrcCNo=0, tdstCNo=0, tfno=0;
	bool firstNeighbor = true;
	
	bestSol = *currPartition;
	bestCost = bestSol.Cost();
	
	for(srcCNo = 0; srcCNo < TotalClusters; srcCNo++)
	{
		for(dstCNo = 0; dstCNo < TotalClusters; dstCNo++)
		{
			if (srcCNo == dstCNo) 
			{
				continue;
			}
			
			TotalFunctions = currPartition->getClusterFunctionCount(srcCNo);
			
			#ifdef RND_INIT_TS
			for(fno=0; fno<TotalFunctions; fno++)
			#else
			for(fno=TotalClusters; fno<TotalFunctions; fno++)	//initial selection is kept same (start from 0 to consider all)			
			#endif
			{
				newBestSol = bestSol;
				newBestSol.removeFunction(fno);
				newBestSol.addFunction(fno,dstCNo);
				
				newBestCost = newBestSol.Cost();
				
				if( (newBestCost < bestCost || firstNeighbor) && (tabuList->getTabuValue(srcCNo,dstCNo,fno) == 0) )
				{
					firstNeighbor = false;
					bestSol = newBestSol;
					bestCost = newBestCost;
					
					tsrcCNo = srcCNo;
					tdstCNo = dstCNo;
					tfno = fno;
				}
			}
		}
	}

	tabuList->tabuMove(tsrcCNo, tdstCNo, tfno);
	tabuList->decrementTabu();		

// 	if(tfno != 0)
// 	{
// 		tabuList->tabuMove(tsrcCNo, tdstCNo, tfno);
// 		tabuList->decrementTabu();		
// 	}

	return bestSol;
}
