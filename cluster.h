#ifndef _CLUSTER_H
#define _CLUSTER_H

#include <set>
#include <iostream>

typedef unsigned int UINT;

using namespace std;

enum Status
{
	UnFinished,
	Finished
};

typedef unsigned int UINT;

class Cluster
{
	private:
		UINT _FunctionCount;
		UINT _FunctionCapacity;
		UINT* _Functions;
		Status _Status;
		
	public:
		Cluster(){_FunctionCount=0; _Status = UnFinished;}
		Cluster(UINT totalFunctions);
		void setFunctionCapacity(UINT totalFunctions);
		void addFunction(UINT fno, UINT cno);
		float ExecCost();
		float ExternalComm();
		float InternalComm();
		bool inCluster(UINT fno);
		
		Status getStatus(){return _Status;}
		void setStatus(Status st) { _Status = st;}

		//Neighbour of a cluster is a node which has a direct inward or outward communication link
		//with the functions of this cluster
		//std::set<UINT> _Neighbours;
		//whenever a function is updated to a cluster, its neighbours are updated
		//later, if we have the functionality of removing functions from cluster,
		//we should also remove this function from neighbours accordingly.
		//secondly, those neighbours which are already a part of some clusters
		//are not considered neighbours here, just for sake of simplicity
		void getNeighbours(std::set<UINT>& Neighbours);
		
		void Print();
};

#endif

