#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "node.h"
#include "edge.h"

class Application
{
	private:
		unsigned int _TotalNodes;
		Node * _Nodes;
		Edges * _Edges;
				
	public:
		Application(){}
		Application(unsigned int nodes);
		
		void print();
};


#endif
