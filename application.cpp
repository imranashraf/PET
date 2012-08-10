#include<iostream>

#include"application.h"
#include"rng.h"

using namespace std;

ULL commLow = 500;
ULL commHigh = 10000;


Application::Application(unsigned int nftns)
{
	RNG rng;
	unsigned int i,j;
	static ULL cummContrib=100;
	ULL contrib;
	
	_TotalFunctions = nftns;
	_Functions = new Function[ _TotalFunctions ];

	for(i=0;i<_TotalFunctions - 1; i++) //got till one less than the total nodes
	{
		contrib = ( abs( rng.rand_int31() ) % (cummContrib + 1 ) );
		_Functions[i].assignContrib(contrib);
		cummContrib -= contrib;
	}
	_Functions[i].assignContrib(cummContrib); //last should get the remaining
	
	
	_Edges = new Edge* [_TotalFunctions];
	if( _Edges == NULL)	{ cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
	
	for(i = 0; i < _TotalFunctions; i++)
	{
		_Edges[i] = new Edge[_TotalFunctions];
		if(_Edges[i] == NULL)	{ cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
	}

	for(i=0 ; i < _TotalFunctions ; i++)
	{
		for(j=0;j< _TotalFunctions;j++)
		{
			//some links will not be there
			_Edges[i][j].setWeight( ( abs( rng.rand_int31() ) % (1 - 0 + 1 ) ) ); 
			
			//the links which are there now should get random communication
			_Edges[i][j].setWeight( _Edges[i][j].getWeight() * ( abs( rng.rand_int31() ) % (commHigh - commLow + 1 ) ) ); 
		}
	}	
}

void Application::print()
{
	unsigned int i,j;
	
	cout<<endl<<"Functions"<<endl;
	for(i=0; i< _TotalFunctions; i++)
		_Functions[i].print();
	
	cout<<endl<<"Edges";

	for(i=0;i < _TotalFunctions;i++)
		cout<<"\t"<<i;
	
	cout<<endl;
	
	for(i=0;i < _TotalFunctions;i++)
	{
		for(j=0;j< _TotalFunctions;j++)
			if(j==0)
				cout<<i<<"\t"<<_Edges[i][j].getWeight();
			else
				cout<<"\t"<<_Edges[i][j].getWeight();
			
			cout<<endl;
	}
	cout<<endl;
	
	
}
