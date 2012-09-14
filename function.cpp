#include <iostream>

#include "globals.h"
#include "function.h"
#include "rng.h"
#include "exception.h"

void Function::setExecContrib(float contrib)
{
	_ExecContrib = contrib;
}

void Function::Print(std::ostream & fout)
{
	fout<<"("<<_FunctionNo<<" , "<<_ExecContrib<<")"<<endl;
}

void Function::CreateRanks()
{
	try
	{
		_Ranks = new double[g_k];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
}
