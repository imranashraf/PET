
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


//Neighbour of a cluster is a node which has a direct inward or outward communication link
//with the functions of this cluster


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
	UINT i,j,cno,fno, indexOfMax;
	
	UINT *Candidates = new UINT[k];
	
	double **Ranks = new double*[k];	//Ranks 2D array will have the ranks of each node for each cluster
	for(i=0;i<k;i++)
		Ranks[i] = new double[n];
	
	InitialSelection(partition, k);
	
	//Phase 1 (Rank Evaluation)
	for	(cno=0; cno<k; cno++) //Iterate over all Clusters with UnFinished Status
	{
		UINT nNeighbours;	
		
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
// 				Ranks[cno][fno] = partition.EvaluateRanks(cno, fno, Neighbours);
// 				std::cout<<"Cluster = "<<cno<<" Neighbour = "<<fno<<endl;
 				Ranks[cno][fno] = EvaluateRanks(cno, fno, Neighbours);
			}
			
			//sort candidates in descending order based on rank
			indexOfMax = Max(&Ranks[cno][0] , n);
			//mark the candidate with the highest rank for merging to cluster cno
			Candidates[cno] = indexOfMax;
		}
	}
	
	//Phase 2 (Conflict Resolution)
	//favor the cluster in which candidate has highest score
	for	(i=0; i<k; i++)
		for	(j=0; j<k; j++)
			if(Candidates[i] == Candidates[j] && (i!=j) )
				Candidates[j] = Ranks[j][0];
				
	
	//Phase 3 (Merging)
	//add the top candidates to the cluster
	for	(cno=0; cno<k; cno++)
	{
		partition.addFunction( Candidates[cno] , cno );
		//cout<<"Add "<<Candidates[cno]<<" to cluster "<<cno<<endl;
	}
	
	cout<<"\nDetails of Partition ..."<<endl;
	partition.Print();
	
}

