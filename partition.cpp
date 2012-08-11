#include<cstdlib>
#include<iostream>
#include<cmath>

#include "partition.h"

const float ALPHA = 1.0;
const float BETA  = 0.5;
const float GAMMA = 0.5;

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

float Partition::BalancingPenalty()
{
	float EC=0.0, BP=0.0;
	float * ECi = new float [_nClusters];
	
	unsigned int i;
	
	for( i=0; i< _nClusters ; i++)
	{
		ECi[i] = _Clusters[i].ExecCost();
		EC += ECi[i];
	}
	
	for( i=0; i< _nClusters ; i++)
	{
		BP += fabs(EC/_nClusters - ECi[i]);
	}
	
	return BP;
}

float Partition::CommunicationCost()
{
	float commcost=0.0;
	unsigned int i;
	
	for( i=0; i< _nClusters ; i++)
	{
		commcost += _Clusters[i].ExternalComm();
	}
	
	return commcost;
}

float Partition::CouplingDegree()
{
	float copdegree=0.0;
	unsigned int i;
	
	for( i=0; i< _nClusters ; i++)
		copdegree += _Clusters[i].InternalComm();

	return copdegree;
}

float Partition::Cost()
{
	float cost=0.0;
	float BP, CC, CD;

	BP = BalancingPenalty();
	CC = CommunicationCost();
	CD = CouplingDegree();
	
 	cost = ALPHA * BP + BETA * CC + GAMMA*1.0/CD;

	return cost;
}

void Partition::Print()
{
	unsigned int i;
	for(i=0;i<_nClusters;i++)
		_Clusters[i].Print();
}