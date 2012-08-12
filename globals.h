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

#ifdef STORE_PARTITIONS
extern Partition * Partitions;
#endif

extern Partition bestPartition;

#ifdef STORE_COSTS
extern float * Costs;
#endif

extern Application * applic;

#endif
