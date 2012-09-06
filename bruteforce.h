#ifndef _BRUTEFORCE_H
#define _BRUTEFORCE_H

#include "globals.h"
#include "algorithm.h"
#include "application.h"
#include "partition.h"

//number of samples of Costs stored for cost distribution analysis
const unsigned int nCOSTSAMPLES = 10e4;

typedef void (*eval_func) (int n, int k, int *partition);

void bruteforce(int n, int k, eval_func eval);
void bruteforce_kfixed(int n, int k, eval_func eval);
void bruteforce_nonempty(int n, int k, eval_func eval);

void print_part (int n, int k, int *partition);

class Bruteforce : public Algorithm
{
	public:
		Bruteforce(){}
		void Apply();
};



#endif