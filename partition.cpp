#include<cstdlib>
#include<iostream>
#include<cmath>
#include<limits>

#include "globals.h"
#include "partition.h"

using namespace std;

Partition::Partition(UINT totalFunctions, UINT nclusters)
{
	ModificationFlag=true;
	_nClusters = nclusters; 

	for(UINT i=0;i<_nClusters;i++)
		_Clusters.push_back( Cluster(totalFunctions) );

	_Cost = std::numeric_limits<double>::quiet_NaN();
}

void Partition::setCluster(UINT totalFunctions, UINT nclusters)
{
	ModificationFlag=true;
	_nClusters = nclusters; 
	_Clusters.resize(_nClusters);
	for(UINT i=0;i<_nClusters;i++)
		_Clusters[i].setFunctionCapacity(totalFunctions);
}

void Partition::addFunction(UINT ftnNo, UINT clusterNo)
{
	ModificationFlag=true;	//a function is added so partition modified
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
			ModificationFlag=true;	//a function is added so partition modified
			break;
		}
	}
}

float Partition::BalancingPenalty()
{
	float EC=0.0, BP=0.0;
	UINT i;
	vector<float>ECi(_nClusters);
	
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
	if(ModificationFlag == true)
	{
		ModificationFlag=false;
		float BP, CC, CD;
		BP = BalancingPenalty();
		CC = CommunicationCost();
		CD = CouplingDegree();
		
		_Cost = ALPHA * BP + BETA * CC + GAMMA*1.0/CD;
	}

	return _Cost;
}

void Partition::Print(std::ostream & fout = std::cout , const std::string & partName = "")
{
	UINT i;
	
	for(i=0;i<_nClusters;i++)
	{
		fout<<" Cluster "<<i<<" has ";
		_Clusters[i].Print(fout);
	}
	
	fout<<fixed;
	
	fout<<partName<<" Cost = "<<Cost()<<endl
		<<partName<<" BP   = "<<BalancingPenalty()<<endl
		<<partName<<" CC   = "<<CommunicationCost()<<endl
		<<partName<<" CD   = "<<CouplingDegree()<<endl;
}
