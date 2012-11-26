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
		bestPartition = new Partition(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	initTemp = 1;
	decayRate = 0.999;
	iterations = 1000;
}

void SimulatedAnnealer::InitialSelection()
{
	UINT fno,cno;
	cout<<"Inside InitialSelection"<<endl;

	for( cno=0; cno<g_k; cno++)
	{
		fno = cno; 	//The functions are sorted in descending order
		currPartition->addFunction(fno,cno);
	}

	for( fno=g_k; fno<g_n; fno++)
	{
		cno = ( abs( rng.rand_int31() ) % ( (g_k-1) - 0 + 1 ) );
		currPartition->addFunction(fno,cno);
	}
	cout<<"Done !!!"<<endl;
}

void SimulatedAnnealer::Apply() 
{
	double temperature = initTemp; /**/
	double nextCost;
	Partition * prevPartition;
	
	cout<<"Inside Apply"<<endl;
	
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
		if (i % 100 == 0) 
		{
			cout<<endl<<i<<"\t"<<minCost<<"\t"<<currCost<<endl;
			currPartition->Print(cout);
		}
		
		prevPartition = currPartition; //save currPartition
		Step(); //step to a new partition (currPartition will be modified)
		
		nextCost = currPartition->Cost();
		
		if (nextCost <= currCost || 
			rng.rand_closed01() /**/ < exp((currCost - nextCost) / temperature)) 
		{
			currCost = nextCost;
			if (nextCost < minCost)
			{
				bestPartition = currPartition;	//clone
				minCost = nextCost;
			}
		}
		else
			currPartition = prevPartition;	//undo
		
		temperature = temperature*decayRate; /**/
	}
	
	cout<<"Cost of the Best Partition found by Simmulated Annealing = "<<minCost<<endl;
	cout<<"Best Partition found by Simmulated Annealing"<<endl;
	bestPartition->Print(cout);
	
	delete prevPartition;
	cout<<"Done !!!"<<endl;
}

void SimulatedAnnealer::Step()
{
	UINT fno, cno;
	cout<<"Inside Step"<<endl;
	
	fno = ( abs( rng.rand_int31() ) % ( (g_n-1) - (g_k) + 1 ) );  //not changing the ftns from initial selection
	cno = ( abs( rng.rand_int31() ) % ( (g_k-1) - 0 + 1 ) );
	
	cout<<"adding function "<<fno<<" to cluster "<<cno<<endl;
	
	currPartition->removeFunction(fno);
	currPartition->addFunction(fno,cno);
	cout<<"Done !!!"<<endl;
}

