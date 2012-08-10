#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "function.h"
#include "edge.h"

class Application
{
	private:
		unsigned int _TotalFunctions;
		Function * _Functions;
		Edge ** _Edges;
				
	public:
		Application(){}
		Application(unsigned int nodes);
		unsigned int getTotalFunctions(){ return _TotalFunctions; };
		unsigned int getFunctionContrib(unsigned int fno){ return _Functions[fno].getExecContrib(); }
		unsigned long long getEdgeWeight(unsigned int i, unsigned int j){return _Edges[i][j].getWeight();}
		void print();
};


#endif
