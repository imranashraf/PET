#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "edge.h"
#include "rng.h"

using namespace std;

void Edge::Print(std::ostream & fout)
{
	fout<<"Weight = "<<_Weight<<endl;
}
