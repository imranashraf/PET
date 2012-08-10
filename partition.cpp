#include<cstdlib>
#include<iostream>

#include "partition.h"

using namespace std;

Partition::Partition(unsigned int totalFunctions, unsigned int nclusters)
{
	_nClusters = nclusters; 
	_Clusters = new Cluster[_nClusters];
	if(_Clusters == NULL)
	{
		cout<<"Could not allocate memory for clusters in partition"<<endl;
		exit(1);
	}
	for(unsigned int i=0;i<_nClusters;i++)
		_Clusters[i].setFunctionCapacity(totalFunctions);
}

void Partition::setCluster(unsigned int totalFunctions, unsigned int nclusters)
{
	_nClusters = nclusters; 
	_Clusters = new Cluster[_nClusters];
	if(_Clusters == NULL)
	{
		cout<<"Could not allocate memory for clusters in partition"<<endl;
		exit(1);
	}
	for(unsigned int i=0;i<_nClusters;i++)
		_Clusters[i].setFunctionCapacity(totalFunctions);
}

void Partition::addFunction(unsigned int ftnNo, unsigned int clusterNo)
{
	_Clusters[clusterNo].addFunction(ftnNo);
}

void Partition::Print()
{
	unsigned int i;
	for(i=0;i<_nClusters;i++)
		_Clusters[i].Print();
}