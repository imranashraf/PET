#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "bruteforce.h"
#include "count.h"
#include "globals.h"

using namespace std;

static int *partition;
static int parCount=0;
long long nPartitions;

static void _Bruteforce(int n)
{
    UINT i;
    // pos n iterate over k -> recursively call _bruteforce with n-1 and k
    if(n==0)
    {
		//store this partition here or do something with it
		Partition tempPart(g_n,g_k);
		
        for(UINT j=0;j<g_n;j++)
		{
 			#ifdef DEBUG
 			cout<<"Function "<<j<<" is in Cluster "<<partition[j]<<endl;
			#endif
			#ifdef STORE_PARTITIONS
			Partitions[parCount].addFunction(j,partition[j]);
			#endif
			
			tempPart.addFunction( j , partition[j] );
		}
		#ifdef DEBUG
 		cout<<endl;
		#endif

		#ifdef STORE_COSTS
		Costs[parCount] = tempPart.Cost();
		#endif

		#ifdef DEBUG
		tempPart.Print();
		cout<<"Cost of Partition = "<<tempPart.Cost()<<endl<<endl;
		#endif
		
		if( (tempPart.Cost() ) < (bestPartition->Cost() ) )
			*bestPartition = tempPart;

		parCount++;
		if(parCount % 100000 == 0)
			cout<<"\rProgress  = "<<setw(3)<<(parCount*100)/nPartitions<<" %"<<flush;
    }
    else
    {
        for(i=0; i<g_k; i++)
        {
            partition[n-1] = i; 
            _Bruteforce(n-1);
        }
    }
}

void Bruteforce_kfixed(int n, int k)
{
    int i;

	partition = new int[n];

    for(i=0;i<n-k;i++)
        partition[i] = 0;
    for(;i<n;i++)
        partition[i] = i+k-n;

    _Bruteforce(n-k);

	delete[] partition;

}

void Bruteforce::Apply()
{
	nPartitions = Count(g_n,g_k);
	bestPartition = new Partition(g_n,g_k);
	
	#ifdef STORE_PARTITIONS
	Partitions = new Partition[nPartitions];
	for(int i=0;i<nPartitions;i++)
		Partitions[i].setCluster(g_n,g_k);
	if( Partitions == NULL )
	{
		cout<<"Could not allocate memory for partitions"<<endl;
		exit(1);
	}
	#endif
	
	#ifdef STORE_COSTS
	Costs = new float[nPartitions];
	#endif
	
	cout<<"Progress  = "<<setw(3)<<(parCount*100)/nPartitions<<" %"<<flush;
	Bruteforce_kfixed( g_n , g_k);
	cout<<endl;
}
