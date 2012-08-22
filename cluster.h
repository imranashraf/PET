#ifndef _CLUSTER_H
#define _CLUSTER_H

#include <set>

// #include "globals.h"
typedef unsigned int UINT;


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
		std::set<UINT> _Neighbours;
		
	public:
		Cluster(){_FunctionCount=0; _Status = UnFinished;}
		Cluster(UINT totalFunctions);
		void setFunctionCapacity(UINT totalFunctions);
		void addFunction(UINT fno);
		float ExecCost();
		float ExternalComm();
		float InternalComm();
		bool inCluster(UINT fno);
		
		Status getStatus(){return _Status;}
		void setStatus(Status st) { _Status = st;}
		
		const std::set<UINT>& getNeighbours() 
		{
			return _Neighbours;
		}
		
// 		void Cluster::UpdateNeighbours();
		
		void Print();
};

#endif

