#include<iostream>

#include"application.h"
#include"rng.h"

using namespace std;

Application::Application(unsigned int nodes)
{
	unsigned int i;
	static ULL cummContrib=100;
	ULL contrib;
	RNG rng;
	
	_TotalFunctions = nodes;
	_Functions = new Function[ _TotalFunctions ];
	_Edges = new Edges(_TotalFunctions);
	
	for(i=0;i<_TotalFunctions - 1; i++) //got till one less than the total nodes
	{
		contrib = ( abs( rng.rand_int31() ) % (cummContrib + 1 ) );
		_Functions[i].assignContrib(contrib);
		cummContrib -= contrib;
	}
	_Functions[i].assignContrib(cummContrib); //last should get the remaining
	
	
	_Edges->assignWeights();
	
}

void Application::print()
{
	unsigned int i;
	
	cout<<endl<<"Functions"<<endl;
	for(i=0; i< _TotalFunctions; i++)
		_Functions[i].print();
	
	cout<<endl<<"Edges";
	_Edges->print();
	
}
