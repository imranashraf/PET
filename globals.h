#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <set>
#include <map>
#include <string>
#include <iostream>

#include "partition.h"
#include "application.h"

//comment the following to hide debugging output
// #define DEBUG

extern std::ostream cnull;

#ifdef DEBUG 
	#define dout (std::cerr)
#else
	#define dout (cnull)
#endif

//comment the following to not store all the partitions produced in bruteforce algorithm
//do it for small simulations only as it will require more memory
// #define STORE_PARTITIONS
// #define PRINT_STORED_PARTITIONS

//comment the following to stop storing the scores of all partions produced in bruteforce algorithm
//do it for small simulations only as it will require more memory and hard disk space
// #define STORE_COSTS

//comment the following if you do not want to run Bruteforce search
#define RUN_BF
//comment the following if you do not want to run Heuristic Search
#define RUN_HS
//comment the following if you do not want to run Simmulated Annealing
// #define RUN_SA
//comment the following if you do not want to run Tabu Search
// #define RUN_TS
//comment the following if you do not want to run Evolutionary Search
#define RUN_ES


//comment the following if you want to show output results on screen
// #define TOFILE

typedef unsigned int UINT;
typedef unsigned long long int ULL;

extern UINT g_n;	//number of functions
extern UINT g_k;	//number of clusters

#ifdef STORE_PARTITIONS
extern Partition* Partitions;
#endif

extern Partition* bestBFPartition;	//best partition found by exhaustive search
extern Partition* bestHSPartition;	//best partition found by heuristic search
extern Partition* bestSAPartition;	//best partition found by Simmulated Annealing
extern Partition* bestTSPartition;	//best partition found by Tabu Search
extern Partition* bestESPartition;	//best partition found by evolutionary search

#ifdef STORE_COSTS
extern float* Costs;
#endif

extern Application* g_applic;
extern Application * g_unfiltered_applic;
extern Application * g_filtered_applic;

extern ULL commLow;
extern ULL commHigh;
extern ULL execLow;
extern ULL execHigh;
extern float PERCENT_CONNECTIVITY;

extern float ALPHA;
extern float BETA;
extern float GAMMA;

//for q2 files name to fno mappings
extern std::set<std::string> SeenFname;
extern std::map <std::string,unsigned int> NametoADD;
extern std::map <unsigned int,std::string> ADDtoName;

//comment the following if you do not want to filter functions in Q2 mode
// #define FILTER

extern unsigned int EXEC_THRESHOLD;
extern unsigned int COMM_THRESHOLD;
extern double TotalFilteredContrib;
extern double TotalFilteredComm;

extern unsigned long long TotalPartitions;
extern unsigned long long PartLimit;
void Pause();

#ifdef RUN_BF
extern double minCost;
extern double maxCost;
#endif

#endif
