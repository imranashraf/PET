#ifndef _CLUSTER_H
#define _CLUSTER_H

#include "node.h"

class Cluster
{
	private:
		unsigned int _NodeCount;
		unsigned int _NodeCapacity;
		Node * _Nodes;
	
	public:
		Cluster(unsigned int totalNodes);
		void addNode(Node node);
		void print();
};

#endif

