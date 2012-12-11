#ifndef EVOLUTIONARYSEARCHER
#define EVOLUTIONARYSEARCHER

#include "algorithm.h"
#include "partition.h"
#include "rng.h"

#define Pm 0.038   
#define Pc 0.6     
#define PoplSize  15 

class EvolutionarySearcher : public Algorithm
{
	private:
		Partition* Population;	//current population
		int Fittest;			//index of the fittest
		int TotalGenerations;	//total number of generations
		RNG rng;
		
	public:
		EvolutionarySearcher();
		void InitialSelection();
		void Eval_Fitness();
		void Find_Fittest();
		void Reproduce();
		void Mutate();
		void Apply();
		void Print(std::ostream & fout);
};

#endif