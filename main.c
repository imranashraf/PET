#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "bruteforce.h"
#include "graph.h"
#include "rng.h"

int main(int argc, char *argv[])
{
	RNG rng;
	int n,k,t;
	Graph g(4);
	if(argc!=3 && argc!=4) {
		printf("syntax: %s n k <t>\n", argv[0]);
		printf("\tn,k: integer\n");
		printf("\t<t>: optional boolean -> k fixed cluster kernels \n");
		return 1;
	}
	
	n = atoi(argv[1]); //number of functions
	k = atoi(argv[2]); //number of clusters
	if(argc==4) t = atoi(argv[3]);
	else t = 0;
	
	if(t==0)
		bruteforce(n,k,print_part);
	else if(t==1)
		bruteforce_kfixed(n,k,print_part);
	else if(t==2)
		bruteforce_nonempty(n,k,print_part);
	
	printf("A random No :  %lu ",rng.rand_int31() );
	
	g.print();
	
	return 0; 
}

