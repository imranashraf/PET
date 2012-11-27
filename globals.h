#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <set>
#include <map>
#include <string>

#include "partition.h"
#include "application.h"

//comment the following to hide debugging output
// #define DEBUG

//comment the following to not store all the partitions produced in bruteforce algorithm
//do it for small simulations only as it will require more memory
// #define STORE_PARTITIONS

//comment the following to stop storing the scores of all partions produced in bruteforce algorithm
//do it for small simulations only as it will require more memory and hard disk space
// #define STORE_COSTS

//uncomment the following if you want to run heuristic only
// #define HEURISTIC_ONLY

//comment the following if you want to show output results on screen
// #define TOFILE

typedef unsigned int UINT;
typedef unsigned long long int ULL;

extern UINT g_n;	//number of functions
extern UINT g_k;	//number of clusters

#ifdef STORE_PARTITIONS
extern Partition* Partitions;
#endif

extern Partition* bestExhPart;	//best partition found by exhaustive search
extern Partition* bestHeurPart;	//partition found by heuristic algorithm
extern Partition* bestSAnPartition;	//partition found by Simmulated Annealing

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
#define FILTER

extern unsigned int EXEC_THRESHOLD;
extern unsigned int COMM_THRESHOLD;
extern double TotalFilteredContrib;
extern double TotalFilteredComm;

void Pause();

#endif
