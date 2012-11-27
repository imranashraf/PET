#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "bruteforce.h"
#include "count.h"
#include "globals.h"
#include "exception.h"

using namespace std;

static int *partition;
unsigned int parCount;
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
			#ifdef STORE_PARTITIONS
			Partitions[parCount].addFunction(j,partition[g_n-j-1]);
			#endif
			tempPart.addFunction( j , partition[g_n-j-1] );
		}
		#ifdef DEBUG
 		cout<<endl;
		#endif

		#ifdef STORE_COSTS
		Costs[parCount%nCOSTSAMPLES] = tempPart.Cost();
		#endif

		#ifdef DEBUG
		tempPart.Print(cout);
		#endif
		
		if( (tempPart.Cost() ) < (bestESPartition->Cost() ) )
			*bestESPartition = tempPart;

		parCount++;
		if(parCount % 100000 == 0)
			cout<<"\rProgress  = "<<setw(3)<<parCount/nPartitions*100.0<<" %"<<flush;
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

	try
	{
		partition = new int[n];
	}
	catch (const std::bad_alloc& e) 
	{
			cout<<e.what()<<endl;
			throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
    for(i=0;i<n-k;i++)
        partition[i] = 0;
    for(;i<n;i++)
        partition[i] = i+k-n;

    _Bruteforce(n-k);
	
	delete[] partition;

}

void Bruteforce::Apply()
{
	parCount=0;
	nPartitions = Count(g_n,g_k);
	
	try
	{
		bestESPartition = new Partition(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	#ifdef STORE_PARTITIONS
	try
	{
		Partitions = new Partition[nPartitions];
		for(int i=0;i<nPartitions;i++)
			Partitions[i].setCluster(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	#endif
	
	#ifdef STORE_COSTS
	//TODO	Think about the solution of such a big array of Costs
	try
	{
		Costs = new float[nCOSTSAMPLES];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	#endif
	
	cout<<"Progress  = "<<setw(3)<<parCount/nPartitions*100.0<<" %"<<flush;
	Bruteforce_kfixed( g_n , g_k);
	cout<<endl;
}
