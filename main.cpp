#include <iostream>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "bruteforce.h"
#include "edge.h"
#include "application.h"
#include "partition.h"
#include "count.h"
#include "globals.h"

using namespace std;

/*void partition_cost(int n, int k, int *partition)
{
	
}*/

Application * applic;

int main(int argc, char *argv[])
{
	int n,k;
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
	
	Algorithm * bforce = new Bruteforce();
	long long totalPartitions = Count(n,k);
	
	bforce->Apply( *applic , k );
	cout<<"Total Partitions Evaluated = "<<totalPartitions<<endl;
	
	#ifdef STORE_PARTITIONS
	cout<<"Partitions evaluated are :"<<endl;
	for(int i=0 ; i < totalPartitions ; i++)
	{
		cout<<endl<<"Partition "<<i<<endl;
		Partitions[i].Print();
	}
	#endif

	#ifdef STORE_COSTS
	cout<<"Costs of evaluated Partitions are ... "<<endl;
	float minCost=Costs[0];
	float maxCost=Costs[0];
	for(int i=0 ; i < totalPartitions ; i++)
	{
		cout<<"\t Partition "<<i<<" Cost = "<<Costs[i]<<endl;
		
		if(Costs[i]<minCost) minCost = Costs[i];
		if(Costs[i]>maxCost) maxCost = Costs[i];
	}
	cout<<"Minimum Cost = "<<minCost<<endl;
	cout<<"Maximum Cost = "<<maxCost<<endl;
	#endif
	
	return 0; 
}

