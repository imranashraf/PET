#include<iostream>

#include"application.h"
#include"rng.h"

using namespace std;

ULL commLow  = 100;
ULL commHigh = 10000;
ULL execLow  = 10;
ULL execHigh = 1000;

Application::Application(unsigned int nftns)
{
	RNG rng;
	unsigned int i,j;
	ULL totalExecTime=0;
	ULL execTime;
	
	_TotalFunctions = nftns;
	_Functions = new Function[ _TotalFunctions ];

	for(i=0;i<_TotalFunctions; i++)
	{
		execTime = ( abs( rng.rand_int31() ) % (execHigh - execLow + 1 ) );
		_Functions[i].setExecContrib(execTime);
		totalExecTime += execTime;
	}
	
	for(i=0;i<_TotalFunctions; i++)
	{
		_Functions[i].setExecContrib( _Functions[i].getExecContrib() / totalExecTime * 100.0);
	}
	
	_Edges = new Edge* [_TotalFunctions];
	if( _Edges == NULL)	{ cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
	
	for(i = 0; i < _TotalFunctions; i++)
	{
		_Edges[i] = new Edge[_TotalFunctions];
		if(_Edges[i] == NULL)	{ cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
	}

	double totalComm=0;
	float comm;
	for(i=0 ; i < _TotalFunctions ; i++)
	{
		for(j=0;j< _TotalFunctions;j++)
		{
			//some links will not be there
			comm = ( abs( rng.rand_int31() ) % (1 - 0 + 1 ) );
		
			//the links which are there now should get random communication
			comm = comm * ( abs( rng.rand_int31() ) % (commHigh - commLow + 1 ) );
			
			_Edges[i][j].setWeight(comm); 
			totalComm += comm;
		}
	}	
	
	for(i=0 ; i < _TotalFunctions ; i++)
	{
		for(j=0;j< _TotalFunctions;j++)
		{
			//some links will not be there
			_Edges[i][j].setWeight( _Edges[i][j].getWeight()/totalComm * 100 ); 
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
