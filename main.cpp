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
	int n,k,t;
	if(argc!=3 && argc!=4) 
	{
		cout<<"syntax: "<<argv[0]<<" n k <t>"<<endl;
		cout<<"\tn,k: integer"<<endl;
		cout<<"\t<t>: optional boolean -> k fixed cluster kernels"<<endl;
		return 1;
	}
	
	n = atoi(argv[1]); //number of functions
	k = atoi(argv[2]); //number of clusters in a partition
	if(argc==4) t = atoi(argv[3]);
	else t = 0;
	
	applic = new Application(n);
	applic->print();
	
	Algorithm * bforce = new Bruteforce();
	long long totalPartitions = Count(n,k);
	
	bforce->Apply( *applic , k );
	
	#ifdef STORE_PARTITIONS
	for(int i=0 ; i < totalPartitions ; i++)
	{
		cout<<endl<<"Partition "<<i<<endl;
		partitions[i].Print();
	}
	#endif
	
	return 0; 
}

