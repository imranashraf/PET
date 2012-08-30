#include<cstdlib>
#include<iostream>
#include<cmath>

#include "globals.h"
#include "partition.h"

using namespace std;

Partition::Partition(UINT totalFunctions, UINT nclusters)
{
	_nClusters = nclusters; 
	_Clusters = new Cluster[_nClusters];
	if(_Clusters == NULL)
	{
		cout<<"Could not allocate memory for clusters in partition"<<endl;
		exit(1);
	}
	for(UINT i=0;i<_nClusters;i++)
		_Clusters[i].setFunctionCapacity(totalFunctions);
}

void Partition::setCluster(UINT totalFunctions, UINT nclusters)
{
	_nClusters = nclusters; 
	_Clusters = new Cluster[_nClusters];
	if(_Clusters == NULL)
	{
		cout<<"Could not allocate memory for clusters in partition"<<endl;
		exit(1);
	}
	for(UINT i=0;i<_nClusters;i++)
		_Clusters[i].setFunctionCapacity(totalFunctions);
}

void Partition::addFunction(UINT ftnNo, UINT clusterNo)
{
	_Clusters[clusterNo].addFunction(ftnNo,clusterNo);
}

float Partition::BalancingPenalty()
{
	float EC=0.0, BP=0.0;
	float * ECi = new float [_nClusters];
	
	UINT i;
	
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
	UINT i;
	
	for( i=0; i< _nClusters ; i++)
	{
		commcost += _Clusters[i].ExternalComm();
	}
	
	return commcost;
}

float Partition::CouplingDegree()
{
	float copdegree=0.0;
	UINT i;
	
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
	UINT i;
	for(i=0;i<_nClusters;i++)
	{
		cout<<" Cluster "<<i<<" has ";
		_Clusters[i].Print();
	}
	
	cout<<fixed;
	cout<<"Cost of Partition = "<<Cost()<<endl;
	cout<<"\tBalancingPenalty = "<<BalancingPenalty()<<endl;
	cout<<"\tCommunicationCost = "<<CommunicationCost()<<endl;
	cout<<"\tCouplingDegree = "<<CouplingDegree()<<endl;
	cout.unsetf(ios::fixed | ios::scientific);
}