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
		float getEdgeWeight(UINT i, UINT j){return _Edges[i][j].getWeight();}
		
		void CreateRanks();
		
		void setClusterNo(UINT fno, UINT cno){ _Functions[fno].setClusterNo(cno); }
		UINT getClusterNo(UINT fno){return _Functions[fno].getClusterNo();}
		
		void Print();
		void Print2Dot();
		
		void Init()
		{
			for(UINT i=0;i<_TotalFunctions ; i++)
			{
				_Functions[i].Init();
			}
			
		}

		void Save();
		
		~Application()
		{
			delete[] _Functions;
			_Functions = NULL;
			
			for(UINT i = 0; i < _TotalFunctions; i++)
			{
				delete[] _Edges[i];
			}
			
			delete[] _Edges;
			_Edges = NULL;
		}
};


#endif
