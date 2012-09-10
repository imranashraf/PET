#include <iostream>

#include "globals.h"
#include "function.h"
#include "rng.h"

void Function::setExecContrib(float contrib)
{
	_ExecContrib = contrib;
}

void Function::Print()
{
	cout<<"("<<_FunctionNo<<" , "<<_ExecContrib<<")"<<endl;
}

void Function::CreateRanks()
{
	_Ranks = new double[g_k];
	if(_Ranks==NULL)
		std::cout<<"Could not allocate memory for Ranks "<<std::endl;
}
