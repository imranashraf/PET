#ifndef _EDGE_H
#define _EDGE_H

#include<cstdlib>

class Edge
{
	private:
		float _Weight;
		
	public:
		Edge(){_Weight=0;}
		Edge(float weight){_Weight = weight;}
		void setWeight(float weight){_Weight = weight;}
		float getWeight(){return _Weight;}
		void Print();
		
};

#endif
