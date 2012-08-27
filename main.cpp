#include <iostream>
#include <climits>
#include <cstdlib>
#include <cmath>

#include "bruteforce.h"
#include "heuristic.h"
#include "edge.h"
#include "application.h"
#include "partition.h"
#include "count.h"
#include "globals.h"
#include "utility.h"

using namespace std;

Application * applic;

int main(int argc, char *argv[])
{
	int n,k;
	Timer *timer = new Timer();
	
	if(argc!=3) 
	{
		cout<<"syntax: "<<argv[0]<<" n k <t>"<<endl;
		cout<<"\tn,k: integer"<<endl;
		return 1;
	}
	
	n = atoi(argv[1]); //number of functions
	k = atoi(argv[2]); //number of clusters in a partition

	applic = new Application(n);
	applic->print();

	/********  Exhaustive Search ********/
	cout<<"====================================";
	cout<<endl<<"Exhaustive Search Summary"<<endl;
	cout<<"===================================="<<endl;
	
	Algorithm * bforce = new Bruteforce();
	long long totalPartitions = Count(n,k);
	
	timer->Start();
	bforce->Apply(k); 
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
	float minCost=Costs[0];
	float maxCost=Costs[0];
	#ifdef DEBUG 
	cout<<"Costs of evaluated Partitions are ... "<<endl;
	#endif
	for(int i=0 ; i < totalPartitions ; i++)
	{
		#ifdef DEBUG 
		cout<<"\t Partition "<<i<<" Cost = "<<Costs[i]<<endl;
		#endif
		
		if(Costs[i]<minCost) minCost = Costs[i];
		if(Costs[i]>maxCost) maxCost = Costs[i];
	}
	cout<<"Minimum Cost = "<<minCost<<endl;
	cout<<"Maximum Cost = "<<maxCost<<endl;
	#endif

	cout<<"Total Partitions Evaluated = "<<totalPartitions<<endl;
	timer->Print(); //print time
	
	cout<<"\nDetails of Best Partition ..."<<endl;
 	bestPartition.Print();
	
	applic->Print2Dot();

	
	/********  Heuristic ********/
	cout<<"====================================";
	cout<<endl<<"Heuristic Search Summary"<<endl;
	cout<<"===================================="<<endl;
	
	Algorithm * heuristic = new Heuristic(k);
 	applic->Init();
	timer->Start();
	heuristic->Apply(k); 
	timer->Stop();
	timer->Print(); //print time
	
// 	cout<<"\nDetails of Partition ..."<<endl;
//  	heurPartition.Print();
	
	return 0; 
}

