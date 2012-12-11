#include <iostream>
#include <cmath>

#include "SimulatedAnnealer.h"
#include "globals.h"

using namespace std;

SimulatedAnnealer::SimulatedAnnealer()
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
		bestSAPartition = new Partition(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	initTemp = 100;
	decayRate = 0.99;
	iterations = 100;
}

void SimulatedAnnealer::InitialSelection()
{
	UINT fno,cno;
	
	#ifdef RND_INIT_SA
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

void SimulatedAnnealer::Apply() 
{
	double temperature = initTemp; /**/
	double nextCost;
	Partition * prevPartition;
	
	InitialSelection();
	currCost= currPartition->Cost();
	minCost = currCost;
	
	try
	{
		prevPartition = new Partition(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	for (int i = 0; i < iterations; i++) 
	{
		dout<<endl<<i<<"\t"<<minCost<<"\t"<<currCost<<endl;
		
		*prevPartition = *currPartition; //save currPartition
		Step(); //step to a new partition (currPartition will be modified)
		
		nextCost = currPartition->Cost();
		
		dout<<rng.rand_closed01()
			<<"\t"
			<<exp( (currCost - nextCost) / temperature)
			<<endl; 
		
		if (nextCost <= currCost || 
			rng.rand_closed01() < exp((currCost - nextCost) / temperature)) /**/
		{
			currCost = nextCost;
			if (nextCost < minCost)
			{
				*bestSAPartition = *currPartition;	//clone
				minCost = nextCost;
			}
		}
		else
			*currPartition = *prevPartition;	//undo
		
		temperature = temperature*decayRate; /**/
	}
	
	delete prevPartition;
}

void SimulatedAnnealer::Step()
{
	UINT fno, cno;

	#ifndef RND_INIT_SA
	fno = 0 + ( abs( rng.rand_int31() ) % ( (g_n-1) - (0) + 1 ) ); 
	#else
	fno = g_k + ( abs( rng.rand_int31() ) % ( (g_n-1) - (g_k) + 1 ) );  //not changing the ftns from initial selection	
	#endif
	cno = 0 + ( abs( rng.rand_int31() ) % ( (g_k-1) - 0 + 1 ) );
	
	currPartition->removeFunction(fno);
	currPartition->addFunction(fno,cno);
}

