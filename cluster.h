#ifndef _CLUSTER_H
#define _CLUSTER_H

#include <set>
#include <iostream>
#include <cstdlib>

typedef unsigned int UINT;

using namespace std;

enum Status
{
	UnFinished,
	Finished
};

class Cluster
{
	private:
		UINT _FunctionCount;
		UINT _FunctionCapacity;
		UINT* _Functions;
		Status _Status;
		
	public:
		Cluster(){_FunctionCount=0; _Status = UnFinished; _Functions=NULL;}
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
		
		
		Cluster& operator=(const Cluster& srcCluster)
		{
			// check for self-assignment
			if (this == &srcCluster)
				return *this;
			
			// first we need to deallocate any value that this string is holding!
			delete[] _Functions;
			
			// because these are not pointers, we can shallow copy it
			_FunctionCount = srcCluster._FunctionCount;
			_FunctionCapacity = srcCluster._FunctionCapacity;
			_Status = srcCluster._Status;
			
			// now we need to deep copy _Clusters
			if (srcCluster._Functions)
			{
				// allocate memory for our copy
				_Functions = new UINT [_FunctionCapacity]; 
				if(_Functions == NULL)
				{
					cout<<"Memory Error"<<endl;
					exit(1);
				}
				
				// Copy the parameter the newly allocated memory
				for(UINT i=0;i<_FunctionCount;i++)
				{
					_Functions[i] = srcCluster._Functions[i];
				}
			}
			else
				_Functions = NULL;
			
			return *this;
		}
		
		~Cluster()
		{
			delete[] _Functions;
			_Functions = NULL;
		}
};

#endif

