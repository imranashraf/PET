#include <iostream>

#include "globals.h"
#include "function.h"
#include "rng.h"
#include "exception.h"

void Function::setExecContrib(float contrib)
{
	_ExecContrib = contrib;
}

void Function::Print(std::ostream & fout = std::cout)
{
	fout<<"("<<_FunctionNo<<","<<_ExecContrib<<")";
}
