#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "bruteforce.h"
#include "count.h"

using namespace std;

Partition * partitions;
static int *partition;
static int g_n, g_k;
static int parCount=0;

static void _Bruteforce(int n)
{
    int i;
    // pos n iterate over k -> recursively call _bruteforce with n-1 and k
    if(n==0)
    {
        //store this partition here
        for(int j=0;j<g_n;j++)
		{
// 			cout<<"Function "<<j<<" is in Cluster "<<partition[j]<<endl;
			partitions[parCount].addFunction(j,partition[j]);
		}
// 		cout<<endl;
		parCount++;
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

    g_n = n;
    g_k = k;

    partition = (int *)malloc( sizeof(int) * n );

    for(i=0;i<n-k;i++)
        partition[i] = 0;
    for(;i<n;i++)
        partition[i] = i+k-n;

    _Bruteforce(n-k);

    free(partition);
}

Partition * Bruteforce::Apply(Application applic , unsigned int k)
{
	unsigned int n = applic.getTotalFunctions();
	long long nPartitions = Count(n,k);
	
	partitions = new Partition[nPartitions];
	for(int i=0;i<nPartitions;i++)
		partitions[i].setCluster(n,k);
	
	if( partitions == NULL )
	{
		cout<<"Could not allocate memory for partitions"<<endl;
		exit(1);
	}
	
	Bruteforce_kfixed( n , k);
	
/*	if(t==0)
		bruteforce(n,k,print_part);
	else if(t==1)
		bruteforce_kfixed(n,k,print_part);
	else if(t==2)
		bruteforce_nonempty(n,k,print_part);
*/

// 	cout<<"Total Partitions = "<<parCount<<endl;
// 	cout<<"combinations = "<<combinations(n,k)<<endl;
// 	cout<<"permutations = "<<permutations(n,k)<<endl;
// 	cout<<"sterling_2nd = "<<sterling_2nd(n,k)<<endl;
// 	cout<<"Count = "<<Count(n,k)<<endl;
	
	
	return partitions;
}

//////////////////////////////////////////
//////////////////////////////////////////

typedef void (*eval_func) (int n, int k, int *partition);
static eval_func evaluate;

static void _bruteforce(int n)
{
    int i;
    // pos n iterate over k -> recursively call _bruteforce with n-1 and k
    if(n==0)
    {
        evaluate(g_n,g_k,partition);
    }
    else
    {
        for(i=0; i<g_k; i++)
        {
            partition[n-1] = i; 
            _bruteforce(n-1);
        }
    }
}

void bruteforce(int n, int k, eval_func eval)
{
    int i;

    g_n = n;
    g_k = k;
    evaluate = eval;
    partition = (int *)malloc(sizeof(int)*n);

    for(i=0;i<n;i++)
        partition[i] = 0;

    _bruteforce(n);

    free(partition);
}

void bruteforce_kfixed(int n, int k, eval_func eval)
{
    int i;

    g_n = n;
    g_k = k;
    evaluate = eval;
    partition = (int *)malloc( sizeof(int) * n );

    for(i=0;i<n-k;i++)
        partition[i] = 0;
    for(;i<n;i++)
        partition[i] = i+k-n;

    _bruteforce(n-k);

    free(partition);
}

void _bruteforce_nonempty(int n, int k)
{
    int i;
    if(n == k) {
        evaluate(g_n,g_k,partition);
    } else {
        for(i=0; i < k; i++)
        {
            partition[n - 1] = i;
            _bruteforce_nonempty(n-1,k);
        }
        partition[n - 1] = n - 1;
        if(k>1)
        {
            partition[n - 1] = k - 1;
            _bruteforce_nonempty(n - 1, k - 1);
            partition[n - 1] = n - 1;
        }
    }
}

void bruteforce_nonempty(int n, int k, eval_func eval)
{
    int i;

    g_n = n;
    g_k = k;
    evaluate = eval;
    partition = (int *)malloc(sizeof(int)*n);

    for(i=0;i<n;i++)
        partition[i] = 0;

    _bruteforce_nonempty(n,k);

    free(partition);
}

void print_part (int n, int k, int *partition)
{
    while(n--) 
        cout<<partition[n];
    
	cout<<endl;
}
