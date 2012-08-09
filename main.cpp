#include <iostream>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "bruteforce.h"
#include "edge.h"
#include "application.h"
#include "cluster.h"

using namespace std;

/*void partition_cost(int n, int k, int *partition)
{
	
}*/

int main(int argc, char *argv[])
{
	int n,k,t,i;
	if(argc!=3 && argc!=4) 
	{
		cout<<"syntax: "<<argv[0]<<" n k <t>"<<endl;
		cout<<"\tn,k: integer"<<endl;
		cout<<"\t<t>: optional boolean -> k fixed cluster kernels"<<endl;
		return 1;
	}
	
	n = atoi(argv[1]); //number of functions
	k = atoi(argv[2]); //number of clusters/partitions
	if(argc==4) t = atoi(argv[3]);
	else t = 0;
	
	Application applic(n);
	applic.print();
	
	Algorithm * algo = new Bruteforce();
	Cluster * clusters;
	clusters = algo->Partition( applic , k );
	
	for(i=0 ; i<k ; i++)
		clusters[i].print();
	
	return 0; 
}

