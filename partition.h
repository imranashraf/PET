#ifndef _PARTITION_H
#define _PARTITION_H

#include <vector>
#include <string>

#include "cluster.h"
#include "exception.h"

typedef unsigned int UINT;

class Partition
{
	private:
		UINT _nClusters;
		std::vector<Cluster> _Clusters;
		float _Cost;
		bool ModificationFlag;
		
	public:
		Partition(){_nClusters=0; ModificationFlag=true;}
		Partition(UINT totalFunctions, UINT nclusters);
	
		UINT getnClusters(){ return _nClusters;}
		
		UINT getClusterFunctionCount(UINT cno)
		{
			return (_Clusters[cno].getFunctionCount() );
		}
		
		void getClusterFunctionNos(UINT cno, std::vector<UINT>& fNos)
		{
			_Clusters[cno].getFunctionNos(fNos);
		}
		
		void setCluster(UINT totalFunctions, UINT nclusters);
		void addFunction(UINT ftnNo, UINT clusterNo);
		void removeFunction(UINT ftnNo);
		
		void Print(std::ostream & fout, const std::string & partName);
		void Print(std::ostream & fout = std::cout)
		{
			Print(fout,"");
		}
		
		float Cost();
		float BalancingPenalty();
		float CommunicationCost();
		float CouplingDegree();

		void setClusterStatus(Status st, UINT cno) {_Clusters[cno].setStatus(st);}
		Status getClusterStatus(UINT cno) {return _Clusters[cno].getStatus();}
		
		void getClusterNeighbours(UINT cno, std::set<UINT> & Neighbours)
		{
			return (_Clusters[cno].getNeighbours(Neighbours) );
		}
};

#endif
