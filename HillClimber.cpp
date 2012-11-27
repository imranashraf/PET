#include <iostream>
#include <cmath>

#include "HillClimber.h"
#include "globals.h"

using namespace std;

HillClimber::HillClimber()
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
		bestHCPartition = new Partition(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	initTemp = 100;
	decayRate = 0.99;
	iterations = 10000;
}

void HillClimber::InitialSelection()
{
	UINT fno,cno;
	
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

void HillClimber::Apply() 
{
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
		#ifdef DEBUG
		if (i % 100 == 0) 
		{
			cout<<endl<<i<<"\t"<<minCost<<"\t"<<currCost<<endl;
		}
		#endif
		
		*prevPartition = *currPartition; //save currPartition
		Step(); //step to a new partition (currPartition will be modified)
		
		nextCost = currPartition->Cost();
		
		#ifdef DEBUG
		cout<<rng.rand_closed01()
			<<"\t"
			<<exp( (currCost - nextCost) / temperature)
			<<endl; 
		#endif
		
		if (nextCost <= currCost) 
		{
			currCost = nextCost;
			if (nextCost < minCost)
			{
				*bestHCPartition = *currPartition;	//clone
				minCost = nextCost;
			}
		}
		else
			*currPartition = *prevPartition;	//undo
	}
	
	delete prevPartition;
}

void HillClimber::Step()
{
	UINT fno, cno;

	fno = g_k + ( abs( rng.rand_int31() ) % ( (g_n-1) - (g_k) + 1 ) );  //not changing the ftns from initial selection
	cno = 0 + ( abs( rng.rand_int31() ) % ( (g_k-1) - 0 + 1 ) );
	
	currPartition->removeFunction(fno);
	currPartition->addFunction(fno,cno);
}

