#include<stdio.h>
#include<stdlib.h>

#include"graph.h"
#include "rng.h"

Graph::Graph(int nodes)
{
	int rows=nodes;
	int cols=nodes;
	int i;
	
	Nodes=nodes;
	
	Weights = (ULL **) malloc (rows * sizeof(ULL *) );
	if(Weights == NULL)	{printf("\n Memory Allocation Error \n"); exit(1);}
	for(i = 0; i < rows; i++)
	{
		Weights[i] = (ULL *)malloc(cols * sizeof(ULL));	
		if(Weights[i] == NULL)	{printf("\n Memory Allocation Error \n"); exit(1);}
	}
}

void Graph::print()
{
	printf("\n");
	
	for(int i=0;i < Nodes;i++)
		printf("  \t %8d  ",i);
	
	printf("\n");
	
	for(int i=0;i < Nodes;i++)
	{
		for(int j=0;j< Nodes;j++)
			if(j==0)
				printf("%d \t %8llu \t ", i , Weights[i][j] );
			else
				printf("%8llu  \t ", Weights[i][j] );
		
		printf("\n");
	}
	printf("\n");
}

void Graph::attachWeights()
{
	RNG rng;
	
	int Low = 500;
	int High = 10000;
		
	for(int i=0;i < Nodes;i++)
		for(int j=0;j< Nodes;j++)
		{
			//some links will not be there
			Weights[i][j] =  ( abs( rng.rand_int31() ) % (1 - 0 + 1 ) ); 
			
			//the links which are there now should get random communication
			Weights[i][j] =  Weights[i][j] * ( abs( rng.rand_int31() ) % (High - Low + 1 ) ); 
		}
	
}
