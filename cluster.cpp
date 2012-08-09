#include <iostream>
#include <cstdlib>

#include "cluster.h"

using namespace std;

Cluster::Cluster(unsigned int totalNodes)
{ 
	_NodeCount = 0; 
	_NodeCapacity = totalNodes;
	
	//for simplicity, we reserve a space equivalent to the space
	//of total number of Nodes in this application.
	_Nodes = new unsigned int [_NodeCapacity]; 
	if(_Nodes == NULL)
	{
		cout<<"Memory Error"<<endl;
		exit(1);
	}
}

void Cluster::addNode(unsigned int node)
{
	if(_NodeCount < _NodeCapacity)
	{
		_Nodes[_NodeCount]=node;
		_NodeCount++;
	}
	else //should not come here in the current implementation
	{
		cout<<"Cluster Full"<<endl;
		exit(1);
	}
}

void Cluster::print()
{
	cout<<"No of Nodes in Cluster = "<<_NodeCount<<endl;
	for(unsigned int i =0; i< _NodeCount; i++)
	{
		cout<<"  Node "<<i<<" = "<<_Nodes[i];
	}
	cout<<endl;
}
