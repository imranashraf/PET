#ifndef _PARTITION_H
#define _PARTITION_H

#include "cluster.h"

typedef unsigned int UINT;

class Partition
{
	private:
		UINT _nClusters;
		Cluster * _Clusters;
		
	public:
		Partition(){_nClusters=0;}
		Partition(UINT totalFunctions, UINT nclusters);
		void setCluster(UINT totalFunctions, UINT nclusters);
		void addFunction(UINT ftnNo, UINT clusterNo);
		void Print();
		float Cost();
		float BalancingPenalty();
		float CommunicationCost();
		float CouplingDegree();

		void setClusterStatus(Status st, UINT cno) {_Clusters[cno].setStatus(st);}
		Status getClusterStatus(UINT cno) {return _Clusters[cno].getStatus();}
		
		const std::set<UINT> & getClusterNeighbours(UINT cno)
		{
			return (_Clusters[cno].getNeighbours() );
		}
		
};


// void UpdateNeighbours()
// {
// 	for(UINT i=0; i< _nClusters ; i++)
// 		_Clusters[i].UpdateNeighbours();
// }


#endif
