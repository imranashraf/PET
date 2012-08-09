#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "bruteforce.h"
#include "edge.h"
#include "application.h"
#include "cluster.h"

int main(int argc, char *argv[])
{
	int n,k,t;
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
	
	Application applic(n);
	applic.print();
	
	Cluster clust(n);
	clust.print();
	
	return 0; 
}

