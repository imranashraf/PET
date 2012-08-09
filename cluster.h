#ifndef _CLUSTER_H
#define _CLUSTER_H

class Cluster
{
	private:
		unsigned int _NodeCount;
		unsigned int _NodeCapacity;
		unsigned int * _Nodes;
	
	public:
		Cluster(){}
		Cluster(unsigned int totalNodes);
		void addNode(unsigned int nodeNo);
		void print();
};

#endif

