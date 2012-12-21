#ifndef EVOLUTIONARYSEARCHER
#define EVOLUTIONARYSEARCHER

#include "algorithm.h"
#include "partition.h"
#include "rng.h"

//comment the following if random initial solution is not required
#define RND_INIT_ES

#define Pm 0.025   
#define Pc 0.75     
#define PoplSize  10 

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