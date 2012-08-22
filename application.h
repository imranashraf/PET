#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "function.h"
#include "edge.h"

typedef unsigned int UINT;

class Application
{
	private:
		Function * _Functions;
		UINT _TotalFunctions;
		Edge ** _Edges;
		double _MaxComm;
		double _TotalComm;
				
	public:
		Application(){}
		Application(UINT nodes);
		UINT getTotalFunctions() const
			{ return _TotalFunctions; }
		float getFunctionContrib(UINT fno){ return _Functions[fno].getExecContrib(); }
		unsigned long long getEdgeWeight(UINT i, UINT j){return _Edges[i][j].getWeight();}
		
		void CreateRanks(UINT k)
		{
			for(UINT i =0 ; i<_TotalFunctions;i++)
				_Functions[i].CreateRanks(k);
		}
		
		void print();
		void Print2Dot();
};


#endif
