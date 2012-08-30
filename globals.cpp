
#include "globals.h"

UINT g_n;	//number of functions
UINT g_k;	//number of clusters

Application * g_applic;		//the application to be partitioned

#ifdef STORE_PARTITIONS
Partition* Partitions;		//the partitions found
#endif

Partition* bestPartition;	//best partition found by exhaustive search
Partition* heurPartition;	//partition found by heuristic algorithm

#ifdef STORE_COSTS
float * Costs;				//the costs of the partitions found by exhaustive search
#endif

ULL commLow  = 4;
ULL commHigh = 1000000;
ULL execLow  = 10;
ULL execHigh = 1000;
float PERCENT_CONNECTIVITY = 50;

float ALPHA = 1.0;
float BETA  = 1.0;
float GAMMA = 1.0;

