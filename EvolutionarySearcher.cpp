#include <iostream>
#include <vector>
#include "globals.h"
#include "EvolutionarySearcher.h"

using namespace std;

EvolutionarySearcher::EvolutionarySearcher()
{
	TotalGenerations = 100;
	Fittest = 0;
	for(int i=0;i<PoplSize;i++)
		Population.push_back( Partition(g_n,g_k) );
// 	Population.resize(PoplSize);
	
// 	try
// 	{
// 		Population = new Partition[PoplSize];
// 		for(int i=0;i<PoplSize;i++)
// 			Population[i].setCluster(g_n,g_k);
// 	}
// 	catch (const std::bad_alloc& e) 
// 	{
// 		cout<<e.what()<<endl;
// 		throw Exception("Allocation Failed",__FILE__,__LINE__);
// 	}
	
	try
	{
		bestESPartition = new Partition(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
}

void EvolutionarySearcher::InitialSelection()
{
	UINT fno,cno;
	
	for(UINT g = 0; g<PoplSize; g++)
	{
		#ifdef RND_INIT_SA
		for( fno=0; fno<g_n; fno++)
		#else
		for( cno=0; cno<g_k; cno++)
		{
			fno = cno; 	//The functions are sorted in descending order
			Population[g].addFunction(fno,cno);
		}
			
		for( fno=g_k; fno<g_n; fno++)
		#endif
		{
			cno = 0 + ( abs( rng.rand_int31() ) % ( (g_k-1) - 0 + 1 ) );
			Population[g].addFunction(fno,cno);
		}
	}
}

void EvolutionarySearcher::Apply()
{
	UINT g;
	
	InitialSelection();
	FindFittest();
	Print(dout);
	
	for(g=0 ; g<TotalGenerations; g++)
	{
		Reproduce();
 		Mutate();
		FindFittest();
	}
	
	*bestESPartition = Population[Fittest];
}

void EvolutionarySearcher::FindFittest()
{
	int i;
	Fittest=0;
	double minCost=Population[Fittest].Cost();
	double currCost;
	
	for(i=1;i<PoplSize;i++)
	{
		currCost = Population[i].Cost();
		if(currCost < minCost )
			Fittest=i;
	}
}

void EvolutionarySearcher::Reproduce()
{
	UINT i;
	UINT fno, cno;
	UINT TotalClusters = g_k;
	vector<UINT> fNos;
	
	for(i=0;i<PoplSize;i++)
	{
		//to not reproduce with itself
		//it would be a waste of time 
		if(i==Fittest) continue;
		
		for(cno=0; cno < TotalClusters; cno++)
		{
			fNos.clear();
			//either the gene will be kept the same or it will be changed
			//to be what the best chromosome has
			Population[Fittest].getClusterFunctionNos(cno, fNos);	//get the traits from fittest
			
			if(fNos.size() <= 1) continue;	//we do not remove the last alone function from a cluster
			
			for (vector<UINT>::iterator it = fNos.begin(); it!=fNos.end(); ++it) 
			{
				if( (rand()%100) > 100*Pc)	continue;	//we do not reproduce

				fno = *it;
				
				//now add that trit to current population
				Population[i].removeFunction(fno);
				Population[i].addFunction(fno,cno);
			}
		}
	}
}

void EvolutionarySearcher::Mutate()
{
	UINT i, TotalFunctions, fno, cno;
	UINT TotalClusters = g_k;

	for(i=0; i<PoplSize; i++)
	{
		for(cno=0; cno < TotalClusters; cno++)
		{
			TotalFunctions = Population[i].getClusterFunctionCount(cno);
		}
	}
	
	return;
	
	for(i=0; i<PoplSize; i++)
	{
		if(i==Fittest) continue;
		
		for(cno=0; cno < TotalClusters; cno++)
		{
			if( (rand()%1000) > 1000*Pm) continue; //tune mutation

			TotalFunctions = Population[i].getClusterFunctionCount(cno);
			
			if(TotalFunctions <= 1) continue;	//we do not remove the last alone function from a cluster
			
			//select a function in the cno cluster for mutation
			#ifndef RND_INIT_ES
			fno = 0 + ( abs( rng.rand_int31() ) % ( (TotalFunctions-1) - (0) + 1 ) ); 
			#else
			fno = g_k + ( abs( rng.rand_int31() ) % ( (TotalFunctions-1) - (g_k) + 1 ) );  //not changing the ftns from initial selection	
			#endif
			
			//mutate it
			Population[i].removeFunction(fno);
			Population[i].addFunction(fno,cno);
		}
	}
}

void EvolutionarySearcher::Print(std::ostream & fout = std::cout)
{
	for(UINT g = 0; g<PoplSize; g++)
	{
		Population[g].Print(fout);
	}
}
