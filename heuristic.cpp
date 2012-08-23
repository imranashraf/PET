#include <list>
#include "heuristic.h"
#include "utility.h"

using namespace std;

void Heuristic::InitialSelection(Partition& partition, UINT k)
{
	UINT fno,cno,n;
	n = applic->getTotalFunctions();
	
	for( cno=0; cno<k; cno++)
	{
		fno = cno; 	//The functions are sorted in descending order
		partition.addFunction( fno , cno );
	}
}

double ExecRank(UINT fno)
{
	return applic->getFunctionContrib(fno);
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
		comm += applic->getEdgeWeight(fnoTemp, fno);	//outwards edge
		comm += applic->getEdgeWeight(fno, fnoTemp);	//inwards  edge
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

void Heuristic::Apply(UINT k)
{
	UINT n = applic->getTotalFunctions();
	Partition partition(n,k);
	UINT cno,cno1,cno2,fno;
	
	int *Marked = new int[k];	//array to hold the marked functions for each cluster
	
	typedef std::list<UINT> CandidateList;
	CandidateList * Candidates = new CandidateList[k];
	std::list<UINT>::iterator itList;
	
	double **Ranks = new double*[k];	//Ranks 2D array will have the rank of each function for each cluster
	for(cno=0;cno<k;cno++)
		Ranks[cno] = new double[n];
	
	InitialSelection(partition, k);
	
	//Phase 1 (Rank Evaluation)
	for	(cno=0; cno<k; cno++) //Iterate over all Clusters with UnFinished Status
	{
		UINT nNeighbours;	
		
		for(cno1=0;cno1<k;cno1++)
			Marked[cno1] = -1; //no function is marked for cno1
			
		if( (partition.getClusterStatus(cno)) == UnFinished)
		{
			//find the neighbouring nodes of cluster cno
			const std::set<UINT> & Neighbours = partition.getClusterNeighbours(cno);
			nNeighbours= Neighbours.size();	
			
			//if no neighbouring node exists, mark cluster as finished
			if(nNeighbours == 0)
			{
				partition.setClusterStatus(Finished,cno);
				continue;
			}
			
			//for all the neightbouring nodes, evaluate its rank for this cluster
			std::set<UINT>::iterator it;
			for(it=Neighbours.begin();it!=Neighbours.end();it++)
			{
				fno = *it; 	//fno of neighbour
 				Ranks[cno][fno] = EvaluateRanks(cno, fno, Neighbours);
				if( Candidates[cno].empty() )
				{
					Candidates[cno].push_back(fno);
				}
				else
				{
					for(itList = Candidates[cno].begin(); itList != Candidates[cno].end(); itList++)
						if( Ranks[cno][*itList] <= Ranks[cno][fno] )
							Candidates[cno].insert(itList,fno);
				}
				
			}
			
			//sort candidates in descending order based on rank 
			//(not needed as already placed in right order)
			
			//the candidate with the highest rank will be marked for merging to cluster cno
			itList = Candidates[cno].begin();		//point to the first element
			Marked[cno] = *itList; //mark it for this cluster cno
		}
	}
	
	//Phase 2 (Conflict Resolution)
	//favor the cluster in which candidate has highest score
	itList = Candidates[cno].begin();
	for	(cno1=0; cno1<k; cno1++)
		for	(cno2=0; cno2<k; cno2++)
			if(Candidates[cno1] == Candidates[cno2] && (cno1 != cno2) )
			{
				cout<<"Resolving Conflict"<<endl;
				Marked[cno2] = -1;	//so cno1 is preffered without any criteria for now
									//and cno2 will not have any marked candidate for merging
									//this can be changed to give priority to highest rank function later
				//Candidates[cno2][*itList] = Candidates[cno2][*itList];
			}
				
	
	//Phase 3 (Merging)
	//add the top candidates to the cluster
	for	(cno=0; cno<k; cno++)
	{
		if(Marked[cno] != -1)	//if there is candidate marked for merging
		{
			partition.addFunction( Marked[cno] , cno );		//merg it to cluster cno
			//cout<<"Add "<<Candidates[cno]<<" to cluster "<<cno<<endl;
		}
	}
	
	cout<<"\nDetails of Partition ..."<<endl;
	partition.Print();
	
}

