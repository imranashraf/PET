#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "cluster.h"
#include "globals.h"

using namespace std;

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
	UINT i;
	
	for(i=0; i<_FunctionCount; i++)
		EC += applic->getFunctionContrib( _Functions[i] );
		
	return EC;
}

Cluster::Cluster(UINT totalFunctions)
{ 
	_FunctionCount = 0; 
	_Status = UnFinished;
	_FunctionCapacity = totalFunctions;
	
	//for simplicity, we reserve a space equivalent to the space
	//of total number of Functions in this application.
	_Functions = new UINT [_FunctionCapacity]; 
	if(_Functions == NULL)
	{
		cout<<"Memory Error"<<endl;
		exit(1);
	}
}

void Cluster::setFunctionCapacity(UINT totalFunctions)
{ 
	_FunctionCount = 0; 
	_FunctionCapacity = totalFunctions;
	
	//for simplicity, we reserve a space equivalent to the space
	//of total number of Functions in this application.
	_Functions = new UINT [_FunctionCapacity]; 
	if(_Functions == NULL)
	{
		cout<<"Memory Error"<<endl;
		exit(1);
	}
}

void Cluster::addFunction(UINT fno , UINT cno)
{
/*	if( applic->getClusterNo(fno) != -1)
	{
		cout<<"Function "<<fno<<" already assigned to Cluster "<<applic->getClusterNo(fno)<<endl;
		exit(0);
	}*/
	
	if(_FunctionCount < _FunctionCapacity)
	{
		_Functions[_FunctionCount]=fno;
		_FunctionCount++;
		applic->setClusterNo(fno, cno);
	}
	else //should not come here in the current implementation
	{
		cout<<"Cluster Full"<<endl;
		exit(1);
	}
	
}

void Cluster::getNeighbours(std::set<UINT>& Neighbours)
{
	UINT fno,i,j;
	UINT n = applic->getTotalFunctions();
	UINT tempCnoj;
	for(i=0; i<_FunctionCount ;i++)	//check for neighbours of all functions in this cluster
	{
		fno = _Functions[i];
		
		for(j=0; j<n ; j++)
		{
			tempCnoj = applic->getClusterNo(j);
/*			cout<<"For "<<fno<<" and "<<j<<endl;
			cout<<"Different = "<< (fno != j) <<endl;
			cout<<"Link = "<<( (applic->getEdgeWeight(fno,j))*100 > 0 || (applic->getEdgeWeight(j,fno))*100 > 0 )<<endl;
			cout<<"Link Out = "<<( (applic->getEdgeWeight(fno,j))*100.0 > 0.0) <<endl;
			cout<<"Weight Out = "<<(applic->getEdgeWeight(fno,j)) <<endl;
			cout<<"Link In = "<<( (applic->getEdgeWeight(j,fno))*100.0 > 0.0 )<<endl;
			cout<<"Weight In = "<<(applic->getEdgeWeight(j,fno))<<endl;
			cout<<"Unused = "<<(tempCnoj==-1)<<endl;*/
			
			if( (fno != j) && 
 				( (applic->getEdgeWeight(fno,j))*100 > 0 || (applic->getEdgeWeight(j,fno))*100 > 0 ) &&
 				tempCnoj == -1
				) 
				{
// 					cout<<"Added"<<endl;
					Neighbours.insert(j);
				}
/*			else
			{
				cout<<"Not Added"<<endl;
			}
			getchar();*/
		}
	}
}


// //remove fno from the neighbours of this cluster if it is already there ...
// _Neighbours.erase(fno);
// 
// //update the set of neighbours of this cluster
// UINT i;
// UINT n = applic->getTotalFunctions();
// 
// for(i=0; i<n ;i++)	//first K are already assigned (can be optimized later)
// {
	// 	if(i==fno) continue;
	// 	
	// 	UINT tempCno = applic->getClusterNo(i);
	// 	//cout<<"fno = "<<i<<" is in cluster "<<tempCno<<endl;
	// 	//if any function has an edge with this function and
	// 	//if this function is not already a part of any cluster 
	// 	//then add it to neighbours
	// 	if( (applic->getEdgeWeight(fno,i)) > 0 && tempCno == -1)
	// 	{
		// 		cout<<"Added fno = "<<fno<<" to cluster "<<cno<<endl;
		// 		_Neighbours.insert(i);
		// 	}
		// }
		

void Cluster::Print()
{
	cout<<_FunctionCount<<" functions ( ";
	for(UINT i =0; i< _FunctionCount; i++)
	{
		cout<<_Functions[i]<<"  ";
	}
	cout<<")"<<endl;
}
