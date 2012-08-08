#include<iostream>

#include"node.h"
#include "rng.h"

using namespace std;

void Node::assignContrib(ULL contrib)
{
	ExecContrib = contrib;
}

void Node::print()
{
	cout<<"("<<NodeNo<<" , "<<ExecContrib<<")"<<endl;
}

