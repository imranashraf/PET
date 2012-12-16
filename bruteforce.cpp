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
unsigned long long partCount;

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
			Partitions[partCount].addFunction(j,partition[g_n-j-1]);
			#endif
			tempPart.addFunction( j , partition[g_n-j-1] );
		}
 		dout<<endl;

		#ifdef STORE_COSTS
		Costs[partCount] = tempPart.Cost();
		#endif

		tempPart.Print(dout);
		
		
		double tempCost = tempPart.Cost();
		if( tempCost <  minCost)
		{
			*bestBFPartition = tempPart;
			minCost = tempCost;
		}
		else if(tempCost > maxCost)
		{
			maxCost = tempCost;
		}
		
		partCount++;
		if(partCount % 100000 == 0)
			cout<<"\rProgress  = "<<(int)( (double)partCount/TotalPartitions*100.0 ) <<" %"<<flush;

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
	partCount=0;
	
	minCost=100000;
	maxCost=0;
	
	try
	{
		bestBFPartition = new Partition(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	#ifdef STORE_PARTITIONS
	try
	{
		Partitions = new Partition[TotalPartitions];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	for(unsigned long long i=0;i<TotalPartitions;i++)
		Partitions[i].setCluster(g_n,g_k);
	#endif
	
	#ifdef STORE_COSTS
	try
	{
		Costs = new float[TotalPartitions];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	#endif
	
	cout<<"Progress  = "<<(int)( (double)partCount/TotalPartitions*100.0 ) <<" %"<<flush;
	Bruteforce_kfixed( g_n , g_k);
	cout<<endl;
}
