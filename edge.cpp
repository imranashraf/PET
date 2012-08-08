#include<iostream>
#include<stdio.h>
#include<stdlib.h>

#include"edge.h"
#include "rng.h"

using namespace std;

ULL commLow = 500;
ULL commHigh = 10000;

Edges::Edges(unsigned int nodes)
{
	_Nodes=nodes;
	unsigned int i;
	
	_Weights = new ULL * [nodes];
	if(_Weights == NULL)	{ cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
	
	for(i = 0; i < _Nodes; i++)
	{
		_Weights[i] = new ULL[_Nodes];
		if(_Weights[i] == NULL)	{ cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
	}
}

void Edges::print()
{
	unsigned int i,j;
	
	printf("\n");
	
	for(i=0;i < _Nodes;i++)
		cout<<"\t"<<i;
	
	cout<<endl;
	
	for(i=0;i < _Nodes;i++)
	{
		for(j=0;j< _Nodes;j++)
			if(j==0)
				cout<<i<<"\t"<<_Weights[i][j];
			else
				cout<<"\t"<<_Weights[i][j];
		
		cout<<endl;
	}
	cout<<endl;
}

void Edges::assignWeights()
{
	RNG rng;
	unsigned int i,j;
	
	for(i=0;i < _Nodes;i++)
		for(j=0;j< _Nodes;j++)
		{
			//some links will not be there
			_Weights[i][j] =  ( abs( rng.rand_int31() ) % (1 - 0 + 1 ) ); 
			
			//the links which are there now should get random communication
			_Weights[i][j] = _Weights[i][j] * ( abs( rng.rand_int31() ) % (commHigh - commLow + 1 ) ); 
		}
}
