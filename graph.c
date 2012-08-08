#include<stdio.h>
#include<stdlib.h>

#include"graph.h"

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
		printf(" \t %d  ",i);
	
	printf("\n");
	
	for(int i=0;i < Nodes;i++)
	{
		for(int j=0;j< Nodes;j++)
			if(j==0)
				printf("%d \t %llu \t ", i , Weights[i][j] );
			else
				printf("%llu  \t ", Weights[i][j] );
		
		printf("\n");
	}
	printf("\n");
}