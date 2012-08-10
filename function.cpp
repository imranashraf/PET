#include<iostream>

#include "function.h"
#include "rng.h"

using namespace std;

void Function::assignContrib(ULL contrib)
{
	ExecContrib = contrib;
}

void Function::print()
{
	cout<<"("<<FunctionNo<<" , "<<ExecContrib<<")"<<endl;
}

