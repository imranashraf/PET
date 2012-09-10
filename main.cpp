#include <iostream>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <fstream>

#include "bruteforce.h"
#include "heuristic.h"
#include "edge.h"
#include "application.h"
#include "partition.h"
#include "count.h"
#include "globals.h"
#include "utility.h"

using namespace std;

int main(int argc, char *argv[])
{
	Timer *timer = new Timer();

	/********  Application Generation Mode (Save/Restore) ********/
	cout<<endl<<"===========================================";
	cout<<endl<<" Application Generation (Save/Restore Mode)";
	cout<<endl<<"==========================================="<<endl;

	if(argc == 1) //Restore Mode (No random application will be generated, instead restored from file)
	{
		cout<<"Restore Mode (No random application will be generated, instead restored from file)"<<endl;
		cout<<"Restoring from file"<<endl;
		g_applic->Restore();
	}
	else if(argc == 3) //Save Mode (random application will be generated and stored to a file for later use)
	{
		cout<<"Save Mode (Random application will be generated and stored to a file for later use)"<<endl;
		g_n = atoi(argv[1]); //number of functions
		g_k = atoi(argv[2]); //number of clusters in a partition
		g_applic = new Application(g_n);
		g_applic->Init();
		if(g_applic == NULL)
		{
			cout<<"Could not allocate memory for g_applic"<<endl;
			exit(1);
		}
		cout<<"Saving application to appData.txt ...";
		g_applic->Save();
		cout<<" Done ! "<<endl;
	}
	else
	{
		cout<<"Syntax 1: Save Mode "<<argv[0]<<" [n k] "<<endl;
		cout<<"\tn (No of functions) ,k (No of clusters) are both integers"<<endl;
		cout<<"\tApplication generated in this case will be stored to appData.txt"<<endl;
		cout<<"Syntax 2: Restore Mode"<<argv[0]<<" [n k] "<<endl;
		cout<<"\t Without n and k, application will be loaded from appData.txt"<<endl;
		exit(1);
	}
	
	/********  Application Summary Print ********/
	cout<<"====================================";
	cout<<endl<<"Application Summary"<<endl;
	cout<<"===================================="<<endl;
	g_applic->Print();

	/********  Exhaustive Search ********/
	cout<<"====================================";
	cout<<endl<<"Exhaustive Search Summary"<<endl;
	cout<<"===================================="<<endl;
	
	Algorithm * bforce = new Bruteforce();
	if(bforce == NULL)
	{
		cout<<"Could not allocate memory for bforce"<<endl;
		exit(1);
	}
	
	long long totalPartitions = Count(g_n,g_k);
	cout<<"Total Partitions to be Evaluated = "<<totalPartitions<<endl;
	cout<<"Approximate Time required to Evaluate "<<totalPartitions
		<<" partitions = "<< EstimateTime(totalPartitions);
		
	timer->Start();
	bforce->Apply(); 
	timer->Stop();
	
	#ifdef STORE_PARTITIONS
	cout<<"Partitions evaluated are :"<<endl;
	for(int i=0 ; i < totalPartitions ; i++)
	{
		cout<<endl<<"Partition "<<i<<endl;
		Partitions[i].Print();
	}
	#endif

	#ifdef STORE_COSTS
	string costfname("costs.txt");
	ofstream costFile;
	costFile.open(costfname.c_str());
	if ( !costFile.good() )
	{
		cout<<"Could not open "<<costfname<<"file"<<endl;
		exit(1);
	}
	
	float minCost=Costs[0];
	float maxCost=Costs[0];
	#ifdef DEBUG 
	cout<<"Costs of evaluated Partitions are ... "<<endl;
	#endif
	
	UINT limit;
	if(totalPartitions < nCOSTSAMPLES)
		limit = totalPartitions;
	else
		limit = nCOSTSAMPLES;
	for(UINT i=0 ; i < limit ; i++)
	{
		#ifdef DEBUG 
		cout<<"\t Partition "<<i<<" Cost = "<<Costs[i]<<endl;
		#endif
		costFile<<Costs[i]<<endl;
		
		if(Costs[i]<minCost) minCost = Costs[i];
		if(Costs[i]>maxCost) maxCost = Costs[i];
	}
	cout<<"Minimum Cost = "<<minCost<<endl;
	cout<<"Maximum Cost = "<<maxCost<<endl;
	
	costFile.close();
	#endif
	
	timer->Print(); //print time
	
	cout<<"\nDetails of Best Partition found by Exhaustive Search ..."<<endl;
 	bestPartition->Print();
	
	g_applic->Print2Dot();
	
	/********  Heuristic ********/
	cout<<"====================================";
	cout<<endl<<"Heuristic Search Summary"<<endl;
	cout<<"===================================="<<endl;
	
	Algorithm * heuristic = new Heuristic();
 	g_applic->Clear();

	timer->Start();
	heuristic->Apply(); 
	timer->Stop();
	timer->Print(); //print time

	cout<<"\nDetails of Partition found by Heuristic Algorithm ..."<<endl;
	heurPartition->Print();
	
	delete bforce;
	delete bestPartition; 
	delete timer;
	delete g_applic;
 	delete heuristic;
	delete heurPartition;
	// 	delete[] Costs;
	return 0; 
}

