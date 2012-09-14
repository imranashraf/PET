#ifndef _GLOBALS_H
#define _GLOBALS_H

#include "partition.h"
#include "application.h"

//comment the following to hide debugging output
// #define DEBUG

//comment the following to not store all the partitions produced in bruteforce algorithm
// #define STORE_PARTITIONS

//comment the following to stop storing the scores of all partions produced in bruteforce algorithm
#define STORE_COSTS

//comment the following if you want to show output results on screen
// #define TOFILE

typedef unsigned int UINT;
typedef unsigned long long int ULL;

extern UINT g_n;	//number of functions
extern UINT g_k;	//number of clusters

#ifdef STORE_PARTITIONS
extern Partition* Partitions;
#endif

extern Partition* bestPartition;	//best partition found by exhaustive search
extern Partition* heurPartition;	//partition found by heuristic algorithm

#ifdef STORE_COSTS
extern float* Costs;
#endif

extern Application* g_applic;

extern ULL commLow;
extern ULL commHigh;
extern ULL execLow;
extern ULL execHigh;
extern float PERCENT_CONNECTIVITY;

extern float ALPHA;
extern float BETA;
extern float GAMMA;

#endif
