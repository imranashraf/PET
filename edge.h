#ifndef _GRAPH_H
#define _GRAPH_H

#include<cstdlib>

typedef unsigned long long ULL;

class Edge
{
	private:
		unsigned int _Weight;
		
	public:
		Edge(){_Weight=0;}
		Edge(unsigned int weight){_Weight = weight;}
		void setWeight(unsigned int weight){_Weight = weight;}
		ULL getWeight(){return _Weight;}
		void print();
		
};

#endif
