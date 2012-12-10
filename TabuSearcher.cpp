#include <iostream>
#include <cmath>

#include "TabuSearcher.h"
#include "globals.h"

using namespace std;

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
	
	iterations = 1000;
}

void TabuSearcher::InitialSelection()
{
	UINT fno, cno;
	
	for( cno=0; cno<g_k; cno++)
	{
		fno = cno; 	//The functions are sorted in descending order
		currPartition->addFunction(fno,cno);
	}

	for( fno=g_k; fno<g_n; fno++)
	{
		cno = 0 + ( abs( rng.rand_int31() ) % ( (g_k-1) - 0 + 1 ) );
		currPartition->addFunction(fno,cno);
	}
}

void TabuSearcher::Apply() 
{
	InitialSelection();
	currCost= currPartition->Cost();
	minCost = currCost;
	*bestTSPartition = *currPartition;
	
	for (int i = 0; i < iterations; i++) 
	{
		*currPartition = getBestNeighbour();
		currCost = currPartition->Cost();
		
		if(currCost < minCost)
		{
			minCost = currCost;
			*bestTSPartition = *currPartition;
			cout<<endl<<"MinCost TS "<<minCost<<endl;
		}
	}
	
}

Partition TabuSearcher::getBestNeighbour()
{
	Partition tempPart(g_n, g_k);
	double tempCost;
	double newMinCost;
	Partition bestNeighbour(g_n, g_k);

	UINT TotalClusters = g_k;
	UINT TotalFunctions;
	UINT srcCNo, dstCNo, fno;
	UINT tsrcCNo=0, tdstCNo=0, tfno=0;
	bool betterFound = false;
	
	newMinCost = minCost;
	
	for(srcCNo = 0; srcCNo < TotalClusters; srcCNo++)
	{
		for(dstCNo = 0; dstCNo < TotalClusters; dstCNo++)
		{
			if (srcCNo == dstCNo) 
			{
				continue;
			}
			
			tempPart = *currPartition;
			TotalFunctions = currPartition->getClusterFunctionCount(srcCNo);
			for(fno=TotalClusters; fno<TotalFunctions; fno++)	//initial selection is kept same (start from 0 to consider all)
			{
				tempPart.removeFunction(fno);
				tempPart.addFunction(fno,dstCNo);
				tempCost = tempPart.Cost();
				if( (tempCost < newMinCost) && (tabuList->getTabuValue(srcCNo,dstCNo,fno) == 0) )
				{
					betterFound = true;
					tsrcCNo = srcCNo;
					tdstCNo = dstCNo;
					tfno = fno;
					bestNeighbour = tempPart;
				}
			}
		}
	}

	tabuList->decrementTabu();
	if(betterFound)
	{
		tabuList->tabuMove(tsrcCNo, tdstCNo, tfno);
	}
	else
	{
		bestNeighbour = *currPartition;
	}

	return bestNeighbour;
}
