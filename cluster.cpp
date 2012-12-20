#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>

#include "cluster.h"
#include "globals.h"
#include "exception.h"

using namespace std;

typedef std::vector<UINT>::iterator cItr;

Cluster::Cluster(UINT fCapacity)
{ 
	_Status = UnFinished;
	_FunctionCapacity = fCapacity;
}

void Cluster::setFunctionCapacity(UINT fCapacity)
{ 
	_FunctionCapacity = fCapacity;
}

void Cluster::addFunction(UINT fno , UINT cno)
{
	if(_Functions.size() < _FunctionCapacity)
	{
		_Functions.push_back(fno);
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
	//Find new end iterator
	vector<UINT>::iterator newEnd = std::remove(_Functions.begin(), _Functions.end(), fno);
	if( newEnd !=  _Functions.end() )
	{
		//Erase the "removed" elements.
		_Functions.erase(newEnd, _Functions.end());
		g_applic->setClusterNo(fno, -1);
	}
}

bool Cluster::inCluster(UINT fno)
{
	if( (find(_Functions.begin(), _Functions.end(), fno)) !=  _Functions.end() )
		return true;
	else
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
	
	for(cItr it=_Functions.begin(); it != _Functions.end(); it++)
	{
		EC += g_applic->getFunctionContrib(*it);
	}
		
	return EC;
}

void Cluster::getNeighbours(std::set<UINT>& Neighbours)
{
	UINT fno,i,j;
	UINT tempCnoj;
	for(i=0; i<_Functions.size() ;i++)	//check for neighbours of all functions in this cluster
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
	fout<<_Functions.size()<<" functions ( ";
	for(cItr it=_Functions.begin(); it != _Functions.end(); it++)
	{
		fout<<*it<<"  ";
	}
	fout<<")"<<endl;
}
