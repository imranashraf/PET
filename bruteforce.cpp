#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <assert.h>
#include <limits>

#include "bruteforce.h"
#include "count.h"
#include "globals.h"
#include "exception.h"

using namespace std;

static int *partition;
double partCount;

static void _Bruteforce(int n)
{
    UINT i;
    // pos n iterate over k -> recursively call _bruteforce with n-1 and k
    if(n==0)
    {
		//store this partition here or do something with it
		#ifdef STORE_PARTITIONS
		Partitions.push_back(Partition(g_n,g_k));
		Partition & tempPart = Partitions.back();
		#else
		Partition tempPart(g_n,g_k);
		#endif
        for(UINT j=0;j<g_n;j++)
		{
			tempPart.addFunction(j, partition[g_n-j-1] );
		}
		
		double tempCost = tempPart.Cost();
		#ifdef STORE_COSTS
		Costs.push_back( tempPart.Cost() );
		#endif
		if( tempCost <  minCost)
		{
			bestBFPartition = tempPart;
			minCost = tempCost;
		}
		else if(tempCost > maxCost)
		{
			maxCost = tempCost;
		}

		dout<<endl;
		tempPart.Print(dout);
		
		partCount++;
		if(fmod(partCount , 100000) == 0)
			cout<<"\rProgress  = "<<(int)( partCount/TotalPartitions*100.0 ) <<" %"<<flush;
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
	
	minCost=numeric_limits<double>::max( );
	maxCost=numeric_limits<double>::min( );
	
	bestBFPartition.setCluster(g_n,g_k);
	
	cout<<"Progress  = "<<(int)( (double)partCount/TotalPartitions*100.0 ) <<" %"<<flush;
	Bruteforce_kfixed( g_n , g_k);
	cout<<endl;
}
