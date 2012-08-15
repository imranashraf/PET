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
		double _MaxComm;
		double _TotalComm;
				
	public:
		Application(){}
		Application(unsigned int nodes);
		unsigned int getTotalFunctions() const
			{ return _TotalFunctions; };
		float getFunctionContrib(unsigned int fno){ return _Functions[fno].getExecContrib(); }
		unsigned long long getEdgeWeight(unsigned int i, unsigned int j){return _Edges[i][j].getWeight();}
		void print();
		void Print2Dot();
};


#endif
