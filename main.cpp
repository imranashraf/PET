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
#include "exception.h"

using namespace std;
void Simulate();

int main(int argc, char *argv[])
{

	cout<<"Starting Simulation"<<endl;

	/********  Application Generation Mode (Save/Restore) ********/
	cout<<endl<<"===========================================";
	cout<<endl<<" Application Generation (Save/Restore Mode)";
	cout<<endl<<"==========================================="<<endl;
	if(argc == 1) //Restore Mode (No random application will be generated, instead restored from file)
	{
		cout<<"Restore Mode (No random application will be generated, instead restored from file)"<<endl;
		cout<<"Restoring from file"<<endl;
		g_applic->Restore();
		
		try
		{
			Simulate();
		}
		catch(Exception &e)
		{
			cerr<<"Exception occurred at "<<e.File()<<":"<<e.Line()<<endl 
			<<"\twith reason:\"" <<e.Reason()<<endl;
		}
	}
	else if(argc == 3) //Save Mode (random application will be generated and stored to a file for later use)
	{
		cout<<"Save Mode (Random application will be generated and stored to a file for later use)"<<endl;
		g_n = atoi(argv[1]); //number of functions
		g_k = atoi(argv[2]); //number of clusters in a partition
		if(g_k >= g_n)
		{
			cout<<"k should be greater than n"<<endl;
			exit(-1);
		}
		
		try
		{
			g_applic = new Application(g_n);
		}
		catch (const std::bad_alloc& e) 
		{
			cerr<<e.what()<<endl;
			throw Exception("Allocation Failed",__FILE__,__LINE__);
		}
		
		g_applic->Init();
		cout<<"Saving application to appData.txt ...";
		g_applic->Save();
		cout<<" Done ! "<<endl;
		
		try
		{
			Simulate();
		}
		catch(Exception &e)
		{
			cerr<<"Exception occurred at "<<e.File()<<":"<<e.Line()<<endl 
			<<"\twith reason:\"" <<e.Reason()<<endl;
		}
	}
	else if(argc == 5) //long run
	{
		cout<<"Long Run Mode"<<endl;
		UINT nLower, nHigher, kLower, kHigher;
		
		nLower  = atoi(argv[1]); 
		nHigher = atoi(argv[2]); 
		kLower  = atoi(argv[3]); 
		kHigher = atoi(argv[4]); 
		
		UINT n,k;

		for(k=kLower; k<=kHigher; k++)
		{
			for(n=nLower; n<=nHigher; n++)			
			{
				if(k>=n)
					continue;

				g_n = n; //number of functions
				g_k = k; //number of clusters in a partition
				try
				{
					g_applic = new Application(g_n);
				}
				catch (const std::bad_alloc& e) 
				{
					cerr<<e.what()<<endl;
					throw Exception("Allocation Failed",__FILE__,__LINE__);
				}
				g_applic->Init();
				
				try
				{
					Simulate();
				}
				catch(Exception &e)
				{
					cerr<<"Exception occurred at "<<e.File()<<":"<<e.Line()<<endl 
					<<"\twith reason:\"" <<e.Reason()<<endl;
				}
			}
		}
	}
	else
	{
		cout<<"Syntax 1: Save Mode "<<argv[0]<<" [n k] "<<endl;
		cout<<"\tn (No of functions) ,k (No of clusters) are both integers"<<endl;
		cout<<"\tApplication generated in this case will be stored to appData.txt"<<endl;
		cout<<"Syntax 2: Restore Mode"<<argv[0]<<" [n k] "<<endl;
		cout<<"\t Without n and k, application will be loaded from appData.txt"<<endl;
		exit(-1);
	}
		
	cout<<"End of Simulation"<<endl;
	return 0; 
}

void Simulate()
{
	Algorithm * heuristic;
	Algorithm * bforce;	
	Timer *timer;
	long long totalPartitions;
	ofstream fout;
	
	//open output file
	string outFileName = MakeFileName("output",g_n,g_k,".txt");
	fout.open(outFileName.c_str());
	if(fout.fail())
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}
	
	//create the timer
	try
	{
		timer = new Timer();
	}
	catch (const std::bad_alloc& e) 
	{
		cerr<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}

	/********  Application Summary Print ********/
	fout<<"====================================";
	fout<<endl<<"Application Summary"<<endl;
	fout<<"===================================="<<endl;
	g_applic->Print(fout);

	/********  Exhaustive Search ********/
	fout<<"====================================";
	fout<<endl<<"Exhaustive Search Summary"<<endl;
	fout<<"===================================="<<endl;

	try
	{
		bforce = new Bruteforce();
	}
	catch (const std::bad_alloc& e) 
	{
		cerr<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}

	totalPartitions = Count(g_n,g_k);
	fout<<"Total Partitions to be Evaluated = "<<totalPartitions<<endl;
	fout<<"Approximate Time required to Evaluate "<<totalPartitions
		<<" partitions = "<< EstimateTime(totalPartitions);

	timer->Start();
	bforce->Apply(); 
	timer->Stop();

	#ifdef STORE_PARTITIONS
	fout<<"Partitions evaluated are :"<<endl;
	for(int i=0 ; i < totalPartitions ; i++)
	{
		fout<<endl<<"Partition "<<i<<endl;
		Partitions[i].Print(fout);
	}
	#endif

	#ifdef STORE_COSTS
	ofstream costFile;
	string costfname = MakeFileName("costs",g_n,g_k,".txt");
	costFile.open(costfname.c_str());
	if (costFile.fail())
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}

	float minCost=Costs[0];
	float maxCost=Costs[0];
	UINT limit;
	
	if(totalPartitions < nCOSTSAMPLES)
		limit = totalPartitions;
	else
		limit = nCOSTSAMPLES;
	for(UINT i=0 ; i < limit ; i++)
	{
		costFile<<Costs[i]<<endl;
		
		if(Costs[i]<minCost) minCost = Costs[i];
		if(Costs[i]>maxCost) maxCost = Costs[i];
	}
	fout<<"Minimum Cost = "<<minCost<<endl;
	fout<<"Maximum Cost = "<<maxCost<<endl;

	costFile.close();
	#endif

	timer->Print(fout); //print time

	fout<<"\nDetails of Best Partition found by Exhaustive Search ..."<<endl;
	bestPartition->Print(fout);

	g_applic->Print2Dot();

	/********  Heuristic ********/
	fout<<"====================================";
	fout<<endl<<"Heuristic Search Summary"<<endl;
	fout<<"===================================="<<endl;

	try
	{
		heuristic = new Heuristic();	
	}
	catch (const std::bad_alloc& e) 
	{
		cerr<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	g_applic->Clear();

	timer->Start();
	heuristic->Apply(); 
	timer->Stop();
	timer->Print(fout); //print time

	fout<<"\nDetails of Partition found by Heuristic Algorithm ..."<<endl;
	heurPartition->Print(fout);

	fout.close();
	delete bforce;
	delete bestPartition; 
	delete timer;
	delete g_applic;
	delete heuristic;
	delete heurPartition;
	delete[] Costs;
}