#include <iostream>
#include <cstdlib>

#include "cluster.h"

using namespace std;

Cluster::Cluster(unsigned int totalFunctions)
{ 
	_FunctionCount = 0; 
	_FunctionCapacity = totalFunctions;
	
	//for simplicity, we reserve a space equivalent to the space
	//of total number of Functions in this application.
	_Functions = new unsigned int [_FunctionCapacity]; 
	if(_Functions == NULL)
	{
		cout<<"Memory Error"<<endl;
		exit(1);
	}
}

void Cluster::setFunctionCapacity(unsigned int totalFunctions)
{ 
	_FunctionCount = 0; 
	_FunctionCapacity = totalFunctions;
	
	//for simplicity, we reserve a space equivalent to the space
	//of total number of Functions in this application.
	_Functions = new unsigned int [_FunctionCapacity]; 
	if(_Functions == NULL)
	{
		cout<<"Memory Error"<<endl;
		exit(1);
	}
}

void Cluster::addFunction(unsigned int node)
{
	if(_FunctionCount < _FunctionCapacity)
	{
		_Functions[_FunctionCount]=node;
		_FunctionCount++;
	}
	else //should not come here in the current implementation
	{
		cout<<"Cluster Full"<<endl;
		exit(1);
	}
}

void Cluster::Print()
{
	cout<<"No of Functions in Cluster = "<<_FunctionCount<<endl;
	for(unsigned int i =0; i< _FunctionCount; i++)
	{
		cout<<"  Function "<<i<<" = "<<_Functions[i];
	}
	cout<<endl;
}
