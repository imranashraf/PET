#include <list>
#include <cstdio>

#include "heuristic.h"
#include "utility.h"
#include "globals.h"
#include "exception.h"

using namespace std;

void Heuristic::InitialSelection()
{
	UINT fno,cno;

	for( cno=0; cno<g_k; cno++)
	{
		fno = cno; 	//The functions are sorted in descending order
		bestHSPartition.addFunction(fno,cno);
	}
}

double ExecRank(UINT fno)
{
	return g_applic->getFunctionContrib(fno);
}

double CommRank(UINT cno, UINT fno , const std::set<UINT> & Neighbours)
{
	double comm=0; 	//percentage communication
	UINT fnoTemp;
	
	//calculate the communication of this fno with the cluster cno
	std::set<UINT>::iterator it;
	for(it=Neighbours.begin();it!=Neighbours.end();it++)
	{
		fnoTemp = *it; 	//fno of neighbour
		comm += g_applic->getEdgeWeight(fnoTemp, fno);	//outwards edge
		comm += g_applic->getEdgeWeight(fno, fnoTemp);	//inwards  edge
	}
	
	return comm;
}

double EvaluateRanks(UINT cno, UINT fno , const std::set<UINT> & Neighbours)
{
	double Rexec=0;
	double Rcomm=0;
	
	Rexec = ExecRank(fno);
	Rcomm = CommRank(cno, fno, Neighbours);
	
	return (Rexec + Rcomm);
}

void Heuristic::Apply()
{
	UINT n=g_n, k=g_k;
	UINT cno,cno1,cno2,fno;
	
	bestHSPartition.setCluster(g_n,g_k);
	
	int *Marked;
	try
	{
		Marked = new int[k];	//array to hold the marked functions for each cluster
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	
	typedef std::list<UINT> CandidateList;
	CandidateList * Candidates;
	
	try
	{
		Candidates = new CandidateList[k];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	std::list<UINT>::iterator itList;
	
	UINT nNeighbours;
	std::set<UINT> Neighbours;
	double **Ranks;
	try
	{
		//Ranks 2D array will have the rank of each function for each cluster
		Ranks = new double* [k];
		for(cno=0;cno<k;cno++)
			Ranks[cno] = new double[n];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	InitialSelection();
	
	UINT finishedClusters=0;	//counter to keep track of finished clusters
	while(finishedClusters < k)
	{
		//Phase 1 (Rank Evaluation)
		for	(cno=0; cno<k; cno++) //Iterate over all Clusters with UnFinished Status
		{
			Marked[cno] = -1; //no function is marked for any cluster
			Candidates[cno].clear();
			
			if( (bestHSPartition.getClusterStatus(cno)) == UnFinished)
			{
				//find the neighbouring nodes of cluster cno
				Neighbours.clear();	//first clear the set
				bestHSPartition.getClusterNeighbours(cno, Neighbours);
				nNeighbours= Neighbours.size();	
				
				//if no neighbouring node exists, mark cluster as finished
				if(nNeighbours == 0)
				{
					bestHSPartition.setClusterStatus(Finished,cno);
					finishedClusters++;
					continue;
				}
				
				//for all the neightbouring nodes, evaluate its rank for this cluster
				std::set<UINT>::iterator it;
				for(it=Neighbours.begin();it!=Neighbours.end();it++)
				{
					fno = *it; 	//fno of neighbour
					
					Ranks[cno][fno] = EvaluateRanks(cno, fno, Neighbours);
					if( Candidates[cno].empty() )	//if there is no candidate in the list
					{
						Candidates[cno].push_back(fno);	//add this fno as a candidate for this cluster
					}
					else	//and if there are already some candidates in the list
							//then we need to insert fno at the proper place based on ranking
							//so the candidate list is sorted in descending order
					{
						for(itList = Candidates[cno].begin(); itList != Candidates[cno].end(); itList++)
							if( Ranks[cno][*itList] <= Ranks[cno][fno] )
								Candidates[cno].insert(itList,fno);
					}
				}
			
				//sort candidates in descending order based on rank 
				//(not needed as already placed in the right order)
				
				//the candidate with the highest rank will be marked for merging to cluster cno
				Marked[cno] = *(Candidates[cno].begin());		//Mark the first element for merging
			}
		}
				
		//Phase 2 (Conflict Resolution)
		//favor the cluster in which candidate has highest score
		for	(cno1=0; cno1<k; cno1++)
		{
			for	(cno2=cno1+1; cno2<k; cno2++)
			{
				if( (Marked[cno1] == Marked[cno2]) && (Marked[cno1] != -1) )
				{
					//Here we are giving priority based on rank of the tempfno for cno1 or cno2
					UINT tempfno = Marked[cno1];	//or Marked[cno2]
					if( Ranks[cno1][tempfno] > Ranks[cno2][tempfno] )
						Marked[cno2] = -1;
					else
						Marked[cno1] = -1;
				}
			}
		}
					
		//Phase 3 (Merging)
		//add the top candidates to the cluster
		for	(cno=0; cno<k; cno++)
		{
			if(Marked[cno] != -1)	//if there is candidate marked for merging
			{
				bestHSPartition.addFunction(Marked[cno],cno);		//merge it to cluster cno
				Marked[cno] = -1;
			}
		}
	}
	
	for(cno=0;cno<k;cno++)
		delete[] Ranks[cno];
	delete[] Ranks; 
	
	delete[] Marked;	Marked=NULL;
	delete[] Candidates; Candidates=NULL;
}

