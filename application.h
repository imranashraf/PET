#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "function.h"
#include "edge.h"

class Application
{
	private:
		unsigned int _TotalFunctions;
		Function * _Functions;
		Edges * _Edges;
				
	public:
		Application(){}
		Application(unsigned int nodes);
		unsigned int getTotalFunctions(){ return _TotalFunctions; };
		
		void print();
};


#endif
