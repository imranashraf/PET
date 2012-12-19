#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

#include "cluster.h"
#include "globals.h"
#include "exception.h"

using namespace std;

Cluster::Cluster(UINT totalFunctions)
{ 
	_FunctionCount = 0; 
	_Status = UnFinished;
	_FunctionCapacity = totalFunctions;
	_Functions.resize(_FunctionCapacity);
}

bool Cluster::inCluster(UINT fno)
{
	UINT i;
	for(i=0; i<_FunctionCount; i++)
		if(_Functions[i] == fno) return true;
	
	return false;
}

float Cluster::ExternalComm()
{
	float comm=0.0;
	UINT i,j;
	
	#pragma omp parallel for reduction(+:comm) private(j)
	for(i=0; i<g_n ;i++)
		for(j=0; j<g_n ; j++)
			if( inCluster(i) == true && inCluster(j) == false ) 
				comm += g_applic->getEdgeWeight(i,j);
		
	return comm;
}

float Cluster::InternalComm()
{
	float comm=0.0;
	UINT i,j;

	#pragma omp parallel for reduction(+:comm) private(j)
	for(i=0; i<g_n ;i++)
		for(j=0; j<g_n ; j++)
			if( inCluster(i) == true && inCluster(j) == true ) 
				comm += g_applic->getEdgeWeight(i,j);
			
	return comm;
}

float Cluster::ExecCost()
{
	float EC=0.0;
	UINT i;
	
	for(i=0; i<_FunctionCount; i++)
	{
		EC += g_applic->getFunctionContrib( _Functions[i] );
	}
		
	return EC;
}

void Cluster::setFunctionCapacity(UINT totalFunctions)
{ 
	_FunctionCount = 0; 
	_FunctionCapacity = totalFunctions;
	_Functions.resize(_FunctionCapacity);
}

void Cluster::addFunction(UINT fno , UINT cno)
{
	if(_FunctionCount < _FunctionCapacity)
	{
		_Functions[_FunctionCount]=fno;
		_FunctionCount++;
		g_applic->setClusterNo(fno, cno);
	}
	else //should not come here in the current implementation
	{
		cout<<"Cluster Full"<<endl;
		exit(1);
	}
	
}

void Cluster::removeFunction(UINT fno)
{
	vector<UINT>::iterator it;
	if( (it = find(_Functions.begin(), _Functions.begin() + _FunctionCount, fno)) !=  _Functions.end() )
	{
		g_applic->setClusterNo(fno, -1);
		rotate(_Functions.begin(), it+1, _Functions.begin() + _FunctionCount);
		_FunctionCount--;
	}
	
}

void Cluster::getNeighbours(std::set<UINT>& Neighbours)
{
	UINT fno,i,j;
	UINT tempCnoj;
	for(i=0; i<_FunctionCount ;i++)	//check for neighbours of all functions in this cluster
	{
		fno = _Functions[i];
		
		for(j=0; j<g_n ; j++)
		{
			tempCnoj = g_applic->getClusterNo(j);
			
			if( (fno != j) && 
 				( (g_applic->getEdgeWeight(fno,j))*100 > 0 || (g_applic->getEdgeWeight(j,fno))*100 > 0 ) &&
 				tempCnoj == (unsigned int)(-1)
				) 
			{
				Neighbours.insert(j);
			}
		}
	}
}

void Cluster::Print(std::ostream & fout = std::cout)
{
	fout<<_FunctionCount<<" functions ( ";
	for(UINT i =0; i< _FunctionCount; i++)
	{
		fout<<_Functions[i]<<"  ";
	}
	fout<<")"<<endl;
}
