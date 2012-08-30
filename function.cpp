#include <iostream>

#include "globals.h"
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

void Function::CreateRanks()
{
	_Ranks = new double[g_k];
	if(_Ranks==NULL)
		std::cout<<"Could not allocate memory for Ranks "<<std::endl;
}
