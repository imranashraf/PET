#ifndef EVOLUTIONARYSEARCHER
#define EVOLUTIONARYSEARCHER

#include "algorithm.h"
#include "partition.h"
#include "rng.h"

#define Pm 0.35   
#define Pc 0.97     
#define PoplSize 25 

class EvolutionarySearcher : public Algorithm
{
	private:
		std::vector<Partition>Population;
		UINT Fittest;			//index of the fittest
		UINT TotalGenerations;	//total number of generations
		RNG rng;
		
	public:
		EvolutionarySearcher();
		void InitialSelection();
		void FindFittest();
		void Reproduce();
		void Mutate();
		void Apply();
		void Print(std::ostream & fout);
};

#endif