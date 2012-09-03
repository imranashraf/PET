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
		
		void getClusterNeighbours(UINT cno, std::set<UINT> & Neighbours)
		{
			return (_Clusters[cno].getNeighbours(Neighbours) );
		}
		
		// Assignment operator
		Partition& operator=(const Partition& srcPart)
		{
			// check for self-assignment
			if (this == &srcPart)
				return *this;
			
			// first we need to deallocate any value that this string is holding!
			delete[] _Clusters;
			
			// because _nClusters is not a pointer, we can shallow copy it
			_nClusters = srcPart._nClusters;
			
			// now we need to deep copy _Clusters
			if (srcPart._Clusters)
			{
				// allocate memory for our copy
				_Clusters = new Cluster[_nClusters];
				if(_Clusters == NULL)
				{
					cout<<"Could not allocate memory for clusters in partition"<<endl;
					exit(1);
				}
				
				// Copy the parameter the newly allocated memory
				for(UINT i=0;i<_nClusters;i++)
				{
					_Clusters[i] = srcPart._Clusters[i];
				}
				
			}
			else
				_Clusters = NULL;
			
			return *this;
		}
		
		~Partition()
		{
			delete[] _Clusters;
			_Clusters = NULL;
		}
};

#endif
