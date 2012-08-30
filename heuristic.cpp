#include <list>
#include <cstdio>
#include "heuristic.h"
#include "utility.h"
#include "globals.h"

using namespace std;

Heuristic::Heuristic()
{ 
	g_applic->CreateRanks();
}

void Heuristic::InitialSelection()
{
	UINT fno,cno;

	for( cno=0; cno<g_k; cno++)
	{
		fno = cno; 	//The functions are sorted in descending order
		heurPartition->addFunction(fno,cno);
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
	heurPartition = new Partition(g_n,g_k);
	
	int *Marked = new int[k];	//array to hold the marked functions for each cluster
	
	typedef std::list<UINT> CandidateList;
	CandidateList * Candidates = new CandidateList[k];
	std::list<UINT>::iterator itList;
	
	UINT nNeighbours;
	std::set<UINT> Neighbours;
	
	double **Ranks = new double*[k];	//Ranks 2D array will have the rank of each function for each cluster
	for(cno=0;cno<k;cno++)
		Ranks[cno] = new double[n];
	
// 	InitialSelection(heurPartition);
	InitialSelection();
	
	UINT finishedClusters=0;	//counter to keep track of finished clusters
	while(finishedClusters < k)
	{
		//Phase 1 (Rank Evaluation)
		for	(cno=0; cno<k; cno++) //Iterate over all Clusters with UnFinished Status
		{
			Marked[cno] = -1; //no function is marked for any cluster
			Candidates[cno].clear();
			
			if( (heurPartition->getClusterStatus(cno)) == UnFinished)
			{
				//find the neighbouring nodes of cluster cno
				Neighbours.clear();	//first clear the set
				heurPartition->getClusterNeighbours(cno, Neighbours);
				nNeighbours= Neighbours.size();	
				
				//if no neighbouring node exists, mark cluster as finished
				if(nNeighbours == 0)
				{
					heurPartition->setClusterStatus(Finished,cno);
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
// 				cout<<"Marked "<<*(Candidates[cno].begin())<<" for cno "<<cno<<endl;
			}
		}
				
		//Phase 2 (Conflict Resolution)
		//favor the cluster in which candidate has highest score
		for	(cno1=0; cno1<k; cno1++)
		{
			for	(cno2=cno1+1; cno2<k; cno2++)
			{
				if( Marked[cno1] == Marked[cno2] )
				{
// 					cout<<"Resolving Conflict, fno = "<<Marked[cno1]<<" for cno = "<<cno1<<" and cno "<<cno2<<endl;
					Marked[cno2] = -1;	//so cno1 is preffered for now, without any criteria 
										//and cno2 will not have any marked candidate for merging
										//this can be changed later to give priority to highest rank function later
				}
			}
		}
					
		//Phase 3 (Merging)
		//add the top candidates to the cluster
		for	(cno=0; cno<k; cno++)
		{
			if(Marked[cno] != -1)	//if there is candidate marked for merging
			{
// 				cout<<"Adding "<<Marked[cno]<<" to cluster "<<cno<<endl;				
				heurPartition->addFunction(Marked[cno],cno);		//merge it to cluster cno
				Marked[cno] = -1;
			}
		}
/*		cout<<endl<<"End of Iteration, finishedClusters = "<<finishedClusters<<endl;
		heurPartition->Print();
		cout<<"Press ENTER to continue ..."<<endl;
		getchar();*/
	}
}

