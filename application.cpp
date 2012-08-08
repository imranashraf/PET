#include<iostream>

#include"application.h"
#include "edge.h"
#include "rng.h"

using namespace std;

Application::Application(unsigned int nodes)
{
	unsigned int i;
	static ULL cummContrib=100;
	ULL contrib;
	RNG rng;
	
	_TotalNodes = nodes;
	_Nodes = new Node[ _TotalNodes ];
	_Edges = new Edges(_TotalNodes);
	
	for(i=0;i<_TotalNodes - 1; i++) //got till one less than the total nodes
	{
		contrib = ( abs( rng.rand_int31() ) % (cummContrib + 1 ) );
		_Nodes[i].assignContrib(contrib);
		cummContrib -= contrib;
	}
	_Nodes[i].assignContrib(cummContrib); //last should get the remaining
	
	
	_Edges->assignWeights();
	
}

void Application::print()
{
	unsigned int i;
	
	cout<<endl<<"Nodes"<<endl;
	for(i=0; i< _TotalNodes; i++)
		_Nodes[i].print();
	
	cout<<endl<<"Edges";
	_Edges->print();
	
}
