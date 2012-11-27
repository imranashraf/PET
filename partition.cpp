#include<cstdlib>
#include<iostream>
#include<cmath>

#include "globals.h"
#include "partition.h"

using namespace std;

Partition::Partition(UINT totalFunctions, UINT nclusters)
{
	_nClusters = nclusters; 
	try
	{
		_Clusters = new Cluster[_nClusters];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	for(UINT i=0;i<_nClusters;i++)
		_Clusters[i].setFunctionCapacity(totalFunctions);
}

void Partition::setCluster(UINT totalFunctions, UINT nclusters)
{
	_nClusters = nclusters; 
	try
	{
		_Clusters = new Cluster[_nClusters];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	for(UINT i=0;i<_nClusters;i++)
		_Clusters[i].setFunctionCapacity(totalFunctions);
}

void Partition::addFunction(UINT ftnNo, UINT clusterNo)
{
	_Clusters[clusterNo].addFunction(ftnNo,clusterNo);
}

void Partition::removeFunction(UINT ftnNo)
{
	UINT i;
	
	for( i=0; i< _nClusters ; i++)
	{
		if( _Clusters[i].inCluster(ftnNo) == true )
		{
			_Clusters[i].removeFunction(ftnNo);
			break;
		}
	}
}

float Partition::BalancingPenalty()
{
	float EC=0.0, BP=0.0;
	float * ECi;
	UINT i;
	
	try
	{
		ECi = new float [_nClusters];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	for( i=0; i< _nClusters ; i++)
	{
		ECi[i] = _Clusters[i].ExecCost();
		EC += ECi[i];
	}
	
	for( i=0; i< _nClusters ; i++)
	{
		BP += fabs(EC/_nClusters - ECi[i]);
	}
	
	delete[] ECi;
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

void Partition::Print(std::ostream & fout)
{
	UINT i;
	for(i=0;i<_nClusters;i++)
	{
		fout<<" Cluster "<<i<<" has ";
		_Clusters[i].Print(fout);
	}
	
	fout<<fixed;
	
	fout<<"Cost = "<<Cost()
		<<" ("
		<<" BP = "<<BalancingPenalty()
		<<" CC = "<<CommunicationCost()
		<<" CD = "<<CouplingDegree()
		<<" )"
		<<endl;
	
// 	fout<<"Cost of Partition = "<<Cost()<<endl;
// 	fout<<"\tBalancingPenalty = "<<BalancingPenalty()<<endl;
// 	fout<<"\tCommunicationCost = "<<CommunicationCost()<<endl;
// 	fout<<"\tCouplingDegree = "<<CouplingDegree()<<endl;
	
	fout.unsetf(ios::fixed | ios::scientific);
}