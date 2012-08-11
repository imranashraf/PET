#include<iostream>

#include "function.h"
#include "rng.h"

using namespace std;

void Function::setExecContrib(float contrib)
{
	_ExecContrib = contrib;
}

void Function::print()
{
	cout<<"("<<_FunctionNo<<" , "<<_ExecContrib<<")"<<endl;
}

