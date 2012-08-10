#ifndef _PARTITION_H
#define _PARTITION_H

#include "cluster.h"

class Partition
{
	private:
		unsigned int _nClusters;
		Cluster * _Clusters;
		
	public:
		Partition(){_nClusters=0;}
		Partition(unsigned int totalFunctions, unsigned int nclusters);
		void setCluster(unsigned int totalFunctions, unsigned int nclusters);
		void addFunction(unsigned int ftnNo, unsigned int clusterNo);
		void Print();
	
};

#endif
