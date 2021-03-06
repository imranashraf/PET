
#include "globals.h"

//null stream for debugging
std::ostream cnull(0);

UINT g_n;	//number of functions
UINT g_k;	//number of clusters

Application* g_applic;		//the application (filtered or orignal) to be partitioned
Application* g_unfiltered_applic;		//the unfiltered application to be partitioned
Application* g_filtered_applic;		//the filtered application to be partitioned

#ifdef STORE_PARTITIONS
std::vector<Partition> Partitions; 	//the partitions found
#endif

Partition bestBFPartition;	//best partition found by exhaustive search
Partition bestHSPartition;	//best partition found by heuristic search
Partition bestSAPartition;	//best partition found by simmulated annealing
Partition bestTSPartition;	//best partition found by Tabu Search
Partition bestESPartition;	//best partition found by evolutionary search

#ifdef STORE_COSTS
std::vector<float> Costs;	//the costs of the partitions found by exhaustive search
#endif

ULL commLow  = 4;
ULL commHigh = 1000000;
ULL execLow  = 10;
ULL execHigh = 100000;
float PERCENT_CONNECTIVITY = 50;

float ALPHA = 1.0;
float BETA  = 1.0;
float GAMMA = 1.0;

//for q2 files name to fno mappings
std::set<std::string> SeenFname;
std::map <std::string,unsigned int> NametoADD;
std::map <unsigned int,std::string> ADDtoName;

//Threshold value (no of functions) selected based on :
unsigned int EXEC_THRESHOLD = 6;	//execution contribution (maip)
unsigned int COMM_THRESHOLD = 6;	//communication contribution (quad)
double TotalFilteredContrib;
double TotalFilteredComm;

void Pause()
{
	cout<<"Press Enter"<<endl;
	getchar();
}

unsigned long long TotalPartitions;
unsigned long long PartLimit=70000000;

double minCost;
double maxCost;
