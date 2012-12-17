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
#include "EvolutionarySearcher.h"

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
	
	cout<<"\nEnd of Simulation"<<endl;
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
	
	#ifdef RUN_BF
	TotalPartitions = Count(g_n,g_k);
	fout<<"Total number of possible partitions= "<<TotalPartitions<<endl;
	fout<<"Approximate Time required to Evaluate these partitions= "<<EstimateTime(TotalPartitions);

	#if defined(STORE_PARTITIONS) || defined(STORE_COSTS)	
	if(TotalPartitions > PartLimit)
		throw Exception("Total number of partitions are above the limit (set for storage)",__FILE__,__LINE__);
	#endif
	
	/********  Bruteforce Search ********/
	fout<<"====================================";
	fout<<endl<<"Bruteforce Search Summary"<<endl;
	fout<<"===================================="<<endl;
	Algorithm * bforce;	
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

	#if defined(STORE_PARTITIONS) && defined(PRINT_STORED_PARTITIONS)
	fout<<"Partitions evaluated are :"<<endl;
	unsigned long long i=0;
	vector<Partition>::iterator iter;
	for(iter=Partitions.begin() ; iter!=Partitions.end() ; iter++)
	{
		fout<<endl<<"Partition "<<i++<<endl;
		iter->Print(fout);
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
	costFile.close();
	#endif
	
	timer->Print(fout); //print time
	
	fout<<"Minimum Cost = "<<minCost<<endl;
	fout<<"Maximum Cost = "<<maxCost<<endl;

	fout<<"\nDetails of Best Partition found by Exhaustive Search ..."<<endl;
	bestBFPartition->Print(fout,"BF");
	
	delete bforce;
	delete bestBFPartition; 
	
	#endif //RUN_BF
	
	#ifdef RUN_HS
	/********  Heuristic Search ********/
	fout<<"====================================";
	fout<<endl<<"Heuristic Search Summary"<<endl;
	fout<<"===================================="<<endl;
	
	Algorithm * heuristic;
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
	bestHSPartition->Print(fout,"HS");

	delete heuristic;
	delete bestHSPartition;
	
	#endif //RUN_HS
	
	#ifdef RUN_SA
	/********  Simulated Annealing ********/
	fout<<"====================================";
	fout<<endl<<"Simmulated Annealing Summary"<<endl;
	fout<<"===================================="<<endl;
	
	Algorithm * sannealer;
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
	bestSAPartition->Print(fout,"SA");

	delete sannealer;
	delete bestSAPartition;
	
	#endif //RUN_SA
	
	#ifdef RUN_TS
	/********  Tabu Search  ********/
	fout<<"====================================";
	fout<<endl<<"Tabu Search Summary"<<endl;
	fout<<"===================================="<<endl;

	Algorithm * tsearcher;
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
	bestTSPartition->Print(fout,"TS");

	delete tsearcher;
	delete bestTSPartition;
	
	#endif //RUN_TS
	
	#ifdef RUN_ES
	/********  Evolutionary Search  ********/
	fout<<"====================================";
	fout<<endl<<"Evolutionary Search Summary"<<endl;
	fout<<"===================================="<<endl;
	
	Algorithm * esearcher;
	try
	{
		esearcher = new EvolutionarySearcher();	
	}
	catch (const std::bad_alloc& e) 
	{
		cerr<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	g_applic->Clear();
	
	timer->Start();
	esearcher->Apply(); 
	timer->Stop();
	timer->Print(fout); //print time
	
	fout<<"\nDetails of Best Partition found by Evolutionary Search..."<<endl;
	bestESPartition->Print(fout,"ES");
	
	delete esearcher;
	delete bestESPartition;
	
	#endif //RUN_ES
	/****************************************/
	
	//closing
	delete timer;
	
	#ifdef TOFILE
	fout.close();
	#endif

	#ifdef STORE_COSTS
	delete[] Costs;
	#endif

 	#ifdef FILTER
 	delete g_filtered_applic;
 	delete g_unfiltered_applic;
	#else
	delete g_applic;
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
