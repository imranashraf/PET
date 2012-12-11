#include<iostream>

#include "globals.h"
#include "EvolutionarySearcher.h"

EvolutionarySearcher::EvolutionarySearcher()
{
	TotalGenerations = 100;
	Fittest = 0;
	
	try
	{
		Population = new Partition[PoplSize];
		for(int i=0;i<PoplSize;i++)
			Population[i].setCluster(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	try
	{
		bestESPartition = new Partition(g_n,g_k);
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
}

void EvolutionarySearcher::InitialSelection()
{
	UINT fno,cno;
	
	for(UINT g = 0; g<PoplSize; g++)
	{
		#ifdef RND_INIT_SA
		for( fno=0; fno<g_n; fno++)
		#else
		for( cno=0; cno<g_k; cno++)
		{
			fno = cno; 	//The functions are sorted in descending order
			Population[g].addFunction(fno,cno);
		}
			
		for( fno=g_k; fno<g_n; fno++)
		#endif
		{
			cno = 0 + ( abs( rng.rand_int31() ) % ( (g_k-1) - 0 + 1 ) );
			Population[g].addFunction(fno,cno);
		}
	}
}

void EvolutionarySearcher::Apply()
{
	InitialSelection();
	Print(cout);
	*bestESPartition = Population[0];
}

void EvolutionarySearcher::Print(std::ostream & fout = std::cout)
{
	for(UINT g = 0; g<PoplSize; g++)
	{
		Population[g].Print(cout);
	}
}

/*

EvolutionarySearcher::Apply()
{
	int g,i;
	
	Init_Population();
	printf("inital population is : \n ");
	for(i=0;i<Array_Size;i++)
	{
		printf("%d ",Population[Fittest][i]);
	}
	printf(" and fitness = %d \n",Population[Fittest][i]);
	getch();
	
	for(g=0 ; g<TotalGenerations; g++)      //g<TotalGenerations && Population[Fittest][i]<End_Value
	{
		Eval_Fitness();
		Find_Fittest();  
		Reproduce();
		Mutate();
		
		printf("\n Currrent Fittest after %d generation is : \n",g);
		for(i=0;i<Array_Size;i++)
		{
			printf("%d ",Population[Fittest][i]);
		}
		printf(" \n and fitness = %d \n",Population[Fittest][Array_Size]);
	}
	printf("\n Fittest Population after %d generations is : \n",g);
	for(i=0;i<Array_Size;i++)
	{
		printf("%d ",Population[Fittest][i]);
	}
	printf(" \n \a\a and fitness = %d \n",Population[Fittest][i]);
}

////////////////////////////////////////////
//initializing population
void InitPopulation() 
{
     int i,j;
     
     for(i=0;i<PoplSize;i++) 
     {
         for(j=0;j<Array_Size;j++)
         {
             Population[i][j]=(1+rand()%Array_Size);
         }
     }

}
////////////////////////////////////////////
//Evaluation
void Eval_Fitness()
{
int i;
     for(i=0;i<PoplSize;i++) 
     {
         Population[i][Array_Size]=BSort(i);//will get fitness from the calculation
     }
}

///////////////////////////////////////////////////////
//Find Best
void Find_Fittest()
{
int i;

for(i=0;i<PoplSize;i++)
    {
    if(Population[i][Array_Size] > Population[Fittest][Array_Size] )
        {
            Fittest=i;
        }
    }
    
}


///////////////////////////////////////////////////////
//Reproduction
void Reproduce()
{
     int i,j;
     
     for(i=0;i<PoplSize;i++)
		{
			//to not reproduce with itself
			//it would be a waste of time 
			//(although this if could introduce a break in the pipeline, well.. forget about this)
			if(i!=Fittest)
			{
				for(j=0;j<Array_Size;j++)
				{
					//either the gene will be kept the same or it will be changed
					//to be what the best chromosome has
					if( (rand()%100) < 100*Pc)
						Population[i][j]=Population[Fittest][j];			
					else
						Population[i][j]=Population[i][j];			
				}
							
			}	
		}
		
}

///////////////////////////////////////////////////////
// Mutate
void Mutate()
{
     int i,j,changed;
     for(i=0;i<PoplSize;i++)
		{
			if(i!=Fittest)
			{
				for(j=0;j<Array_Size;j++)
				{
                    if( (rand()%1000) < 1000*Pm)//tune mutation
                        {
                        Population[i][j]=(1+rand()%Array_Size);
                        }
                        
				}
							
			}	
		}
     
}

*/
