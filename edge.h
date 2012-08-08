#ifndef _GRAPH_H
#define _GRAPH_H

typedef unsigned long long ULL;

class Edges
{
	private:
		unsigned int _Nodes;
		ULL ** _Weights;
		
	public:
		Edges(){_Nodes=0; _Weights=NULL;}
		Edges(unsigned int nodes);
		void assignWeights();
		void print();
		
};

#endif
