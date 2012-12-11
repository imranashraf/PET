#include <iostream>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <exception>

#include "bruteforce.h"
#include "heuristic.h"
#include "edge.h"
#include "application.h"
#include "partition.h"
#include "count.h"
#include "globals.h"
#include "utility.h"
#include "exception.h"
#include "SimulatedAnnealer.h"
#include "TabuSearcher.h"

using namespace std;
void SetSimulation(int argc, char * argv[]);
void Simulate();
void usage();

int main(int argc, char *argv[])
{
	cout<<"Starting Simulation"<<endl;
	
	try
	{
		SetSimulation(argc,argv);
	}
	catch(Exception &e)
	{
		cerr<<"Exception occurred at "<<e.File()<<":"<<e.Line()<<endl 
		<<"\twith reason:\"" <<e.Reason()<<endl;
	}
	
	cout<<"End of Simulation"<<endl;
	return 0; 
}

void SetSimulation(int argc, char * argv[])
{
	string fname, maipfname, quadfname;
	int Mode;
	
	if(argc>=2)
		Mode = atoi(argv[1]);
	else
	{
		usage();
		throw Exception("Invalid command line arguments",__FILE__,__LINE__);
	}
	
	/********  Application Generation Mode (Save/Restore) ********/
	cout<<endl<<"===========================================";
	cout<<endl<<" Application Generation (Save/Restore Mode)";
	cout<<endl<<"==========================================="<<endl;
	switch(Mode)
	{
		case 1:
			cout<<"Save Mode (Random application will be generated and stored to a file for later use)"<<endl;
			if(argc != 4) //Random Mode (Random application will be generated and stored to a file)
				throw Exception("Invalid command line arguments",__FILE__,__LINE__);
			
			g_n = atoi(argv[2]); //number of functions
			g_k = atoi(argv[3]); //number of clusters in a partition
			if(g_k >= g_n)
				throw Exception("g_n should be greater than g_k",__FILE__,__LINE__);
			
			//create application
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
		break;
		//////////////////////////////////////////////////////////////////////
		
		case 2:	//Restore Mode (Application will be restored from file)
			cout<<"Restore Mode (Application will be restored from file)"<<endl;
			if(argc != 4) 
				throw Exception("Invalid command line arguments",__FILE__,__LINE__);
			
			fname = argv[2];
			g_k = atoi(argv[3]);
			g_n = get_g_n(fname); //get g_n according to the number of functions in appData.txt file
			if(g_k >= g_n)
				throw Exception("g_n should be greater than g_k",__FILE__,__LINE__);
			
			//create application
			try
			{
				g_applic = new Application(g_n);
			}
			catch (const std::bad_alloc& e) 
			{
				cerr<<e.what()<<endl;
				throw Exception("Allocation Failed",__FILE__,__LINE__);
			}

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
		break;
		//////////////////////////////////////////////////////////////////////
		case 3:
			cout<<"Restore Q2 Mode (Application will be restored from maip and quad data files)"<<endl;
			if(argc != 5) 
				throw Exception("Invalid command line arguments",__FILE__,__LINE__);
			
			maipfname = argv[2]; 
			quadfname = argv[3];
			g_k = atoi(argv[4]); //number of clusters
			g_n = getq2_g_n(maipfname, quadfname); //get g_n from maip and quad files
			if(g_k >= g_n)
				throw Exception("g_n should be greater than g_k",__FILE__,__LINE__);
			
			//create application
			try
			{
				g_applic = new Application(g_n);
			}
			catch (const std::bad_alloc& e) 
			{
				cerr<<e.what()<<endl;
				throw Exception("Allocation Failed",__FILE__,__LINE__);
			}
			
			//read q2 files to restore application data
			cout<<"Restoring from file ...";
			g_applic->RestoreQ2();
			cout<<"Done !"<<endl;
			
			#ifdef FILTER
			//store the current application
			g_unfiltered_applic = g_applic;
			g_applic->Filter();
			
			g_applic = g_filtered_applic;
			g_n=g_applic->getTotalFunctions();
			#endif
			
			//start simulation
			try
			{
				Simulate();
			}
			catch(Exception &e)
			{
				cerr<<"Exception occurred at "<<e.File()<<":"<<e.Line()<<endl 
				<<"\twith reason:\"" <<e.Reason()<<endl;
			}
		break;
		//////////////////////////////////////////////////////////////////////
		case 4:
			UINT nLower, nHigher, kLower, kHigher;
			UINT n,k;			
			cout<<"Long Run Mode"<<endl;
			if(argc != 6) 
				throw Exception("Invalid command line arguments",__FILE__,__LINE__);

			nLower  = atoi(argv[2]); 
			nHigher = atoi(argv[3]); 
			kLower  = atoi(argv[4]); 
			kHigher = atoi(argv[5]); 

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
			
		break;
		//////////////////////////////////////////////////////////////////////
		default:
			usage();
			throw Exception("Invalid command line arguments",__FILE__,__LINE__);
		break;
	}
}

void Simulate()
{
	Algorithm * heuristic;
	Algorithm * sannealer;
	Algorithm * tsearcher;
	
	#ifdef RUN_EXHAUSTIVE
	Algorithm * bforce;	
	#endif
	
	Timer *timer;
	
	#ifdef TOFILE
	//open output file
	ofstream fout;
	string outFileName = MakeFileName("output",g_n,g_k,".txt");
	fout.open(outFileName.c_str());
	if(fout.fail())
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}
	#else
	ostream & fout = cout;
	#endif

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
	g_applic->Print2Dot();
	
	#ifdef FILTER
	fout<<"Execution contribution of filtered functions "<<TotalFilteredContrib<<endl;
	fout<<"Communication of filtered functions "<<TotalFilteredComm<<endl;
	#endif
	
	#ifdef RUN_EXHAUSTIVE
	unsigned long long totalPartitions;
	totalPartitions = Count(g_n,g_k);
	fout<<"Total number of possible partitions= "<<totalPartitions<<endl;
	fout<<"Approximate Time required to Evaluate these partitions = "<< EstimateTime(totalPartitions);
	
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

	timer->Start();
	bforce->Apply(); 
	timer->Stop();

	#ifdef STORE_PARTITIONS
	unsigned long long totalPartitions;
	totalPartitions = Count(g_n,g_k);
	fout<<"Partitions evaluated are :"<<endl;
	for(unsigned int i=0 ; i < totalPartitions ; i++)
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
	bestESPartition->Print(fout);
	#endif
	
	
	/********  Heuristic Search ********/
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

	fout<<"\nDetails of Best Partition found by Heuristic Algorithm ..."<<endl;
	bestHSPartition->Print(fout);

	/********  Simulated Annealing ********/
	fout<<"====================================";
	fout<<endl<<"Simmulated Annealing Summary"<<endl;
	fout<<"===================================="<<endl;

	try
	{
		sannealer = new SimulatedAnnealer();	
	}
	catch (const std::bad_alloc& e) 
	{
		cerr<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	g_applic->Clear();

	timer->Start();
	sannealer->Apply(); 
	timer->Stop();
	timer->Print(fout); //print time

	fout<<"\nDetails of Best Partition found by Simmulated Annealing..."<<endl;
	bestSAPartition->Print(fout);

	/********  Tabu Search  ********/
	fout<<"====================================";
	fout<<endl<<"Tabu Search Summary"<<endl;
	fout<<"===================================="<<endl;
	
	try
	{
		tsearcher = new TabuSearcher();	
	}
	catch (const std::bad_alloc& e) 
	{
		cerr<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	g_applic->Clear();
	
	timer->Start();
	tsearcher->Apply(); 
	timer->Stop();
	timer->Print(fout); //print time
	
	fout<<"\nDetails of Best Partition found by Tabu Search..."<<endl;
	bestTSPartition->Print(fout);
	
	/****************************************/
	
	//closing
	#ifdef TOFILE
	fout.close();
	#endif
	#ifdef RUN_EXHAUSTIVE
	delete bforce;
	#endif
	delete bestESPartition; 
	delete timer;
	
	delete sannealer;
	delete bestSAPartition;
	
	delete bestHCPartition;

	delete tsearcher;
	delete bestTSPartition;
	
 	#ifdef FILTER
 	delete g_filtered_applic;
 	delete g_unfiltered_applic;
	#else
	delete g_applic;
	#endif
	
	delete heuristic;
	delete bestHSPartition;
	
	#ifdef STORE_COSTS
	delete[] Costs;
	#endif
}

void usage()
{
	/*
	1 Random Mode		< argc == 4 >	./partool MODE(1) <n> <k>
	2 Restore Mode		< argc == 4 >	./partool MODE(2) <appFileName> <k>
	3 Q2 Mode			< argc == 5 >	./partool MODE(3) <maipFileName> <quadFileName> <k>
	4 Long Run Mode		< argc == 6 >	./partool MODE(4) <nLower> <nHigher> <kLower> <kHigher>
	*/
	cout<<"Mode 1: Random Mode  \n  ./partool 1 <n> <k> "<<endl;
	cout<<"Mode 2: Restore Mode \n  ./partool 2 <appFileName> <k> "<<endl;
	cout<<"Mode 3: Q2 Mode \n  ./partool 3 <maipFileName> <quadFileName> <k>"<<endl;
	cout<<"Mode 4: Long Run Mode \n  ./partool 4 <nLower> <nHigher> <kLower> <kHigher>"<<endl;
}
