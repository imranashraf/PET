#include <iostream>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <fstream>

#include "bruteforce.h"
#include "heuristic.h"
#include "edge.h"
#include "application.h"
#include "partition.h"
#include "count.h"
#include "globals.h"
#include "utility.h"

using namespace std;

int main(int argc, char *argv[])
{
	Timer *timer = new Timer();
	
	if(argc!=3) 
	{
		cout<<"syntax: "<<argv[0]<<" n k <t>"<<endl;
		cout<<"\tn,k: integer"<<endl;
		return 1;
	}
	
	g_n = atoi(argv[1]); //number of functions
	g_k = atoi(argv[2]); //number of clusters in a partition
	g_applic = new Application(g_n);
	if(g_applic == NULL)
	{
		cout<<"Could not allocate memory for g_applic"<<endl;
		exit(1);
	}

	
	/********  Application Summary Print ********/
	cout<<"====================================";
	cout<<endl<<"Application Summary"<<endl;
	cout<<"===================================="<<endl;
	g_applic->Print();

	/********  Exhaustive Search ********/
	cout<<"====================================";
	cout<<endl<<"Exhaustive Search Summary"<<endl;
	cout<<"===================================="<<endl;
	
	Algorithm * bforce = new Bruteforce();
	if(bforce == NULL)
	{
		cout<<"Could not allocate memory for bforce"<<endl;
		exit(1);
	}
	
	long long totalPartitions = Count(g_n,g_k);
	cout<<"Total Partitions to be Evaluated = "<<totalPartitions<<endl;
	cout<<"Approximate Time required to Evaluate "<<totalPartitions
		<<" partitions = "<< EstimateTime(totalPartitions);
		
	timer->Start();
	bforce->Apply(); 
	timer->Stop();
	
	#ifdef STORE_PARTITIONS
	cout<<"Partitions evaluated are :"<<endl;
	for(int i=0 ; i < totalPartitions ; i++)
	{
		cout<<endl<<"Partition "<<i<<endl;
		Partitions[i].Print();
	}
	#endif

	#ifdef STORE_COSTS
	ofstream costFile("costs.txt");
	float minCost=Costs[0];
	float maxCost=Costs[0];
	#ifdef DEBUG 
	cout<<"Costs of evaluated Partitions are ... "<<endl;
	#endif
	
	UINT limit;
	if(totalPartitions < nCOSTSAMPLES)
		limit = totalPartitions;
	else
		limit = nCOSTSAMPLES;
	for(UINT i=0 ; i < limit ; i++)
	{
		#ifdef DEBUG 
		cout<<"\t Partition "<<i<<" Cost = "<<Costs[i]<<endl;
		#endif
		costFile<<Costs[i]<<endl;
		
		if(Costs[i]<minCost) minCost = Costs[i];
		if(Costs[i]>maxCost) maxCost = Costs[i];
	}
	cout<<"Minimum Cost = "<<minCost<<endl;
	cout<<"Maximum Cost = "<<maxCost<<endl;
	
	costFile.close();
	#endif
	
	timer->Print(); //print time
	
	cout<<"\nDetails of Best Partition found by Exhaustive Search ..."<<endl;
 	bestPartition->Print();
	
	g_applic->Print2Dot();
	
	/********  Heuristic ********/
	cout<<"====================================";
	cout<<endl<<"Heuristic Search Summary"<<endl;
	cout<<"===================================="<<endl;
	
	Algorithm * heuristic = new Heuristic();
 	g_applic->Init();

	timer->Start();
	heuristic->Apply(); 
	timer->Stop();
	timer->Print(); //print time

	cout<<"\nDetails of Partition found by Heuristic Algorithm ..."<<endl;
	heurPartition->Print();
	
	delete bforce;
	delete bestPartition; 
	delete timer;
	delete g_applic;
 	delete heuristic;
	delete heurPartition;
	// 	delete[] Costs;
	return 0; 
}

