#ifndef _GRAPH_H
#define _GRAPH_H

typedef unsigned long long ULL;

class Graph
{
	private:
		int Nodes;
		ULL **Weights;
		
	public:
		Graph(){Nodes=0; Weights=NULL;}
		Graph(int nodes);
		void attachWeights();
		void print();
		
};

#endif
