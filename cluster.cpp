#include <iostream>
#include <cstdlib>

#include "cluster.h"
#include "globals.h"

using namespace std;

bool Cluster::inCluster(unsigned int fno)
{
	unsigned int i;
	for(i=0; i<_FunctionCount; i++)
		if(_Functions[i] == fno) return true;
	
	return false;
}

float Cluster::ExternalComm()
{
	float comm=0.0;
	int i,j;
	int n = applic->getTotalFunctions();
	
	for(i=0; i<n ;i++)
		for(j=0; j<n ; j++)
			if( inCluster(i) == true && inCluster(j) == false ) 
				comm += applic->getEdgeWeight(i,j);
		
	return comm;
}

float Cluster::InternalComm()
{
	float comm=0.0;
	int i,j;
	int n = applic->getTotalFunctions();
	
	for(i=0; i<n ;i++)
		for(j=0; j<n ; j++)
			if( inCluster(i) == true && inCluster(j) == true ) 
				comm += applic->getEdgeWeight(i,j);
			
			return comm;
}

float Cluster::ExecCost()
{
	float EC=0.0;
	unsigned int i;
	
	for(i=0;i<_FunctionCount;i++)
		EC += applic->getFunctionContrib( _Functions[i] );
		
	return EC;
}

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

void Cluster::addFunction(unsigned int fno)
{
	if(_FunctionCount < _FunctionCapacity)
	{
		_Functions[_FunctionCount]=fno;
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
	cout<<_FunctionCount<<" functions in Cluster ( ";
	for(unsigned int i =0; i< _FunctionCount; i++)
	{
		cout<<_Functions[i]<<"  ";
	}
	cout<<")"<<endl;
}
