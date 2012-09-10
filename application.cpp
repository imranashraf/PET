#include <iostream>
#include <iomanip>
#include <fstream>

#include "application.h"
#include "rng.h"
#include "utility.h"
#include "globals.h"

using namespace std;

Application::Application(unsigned int nftns)
{
	RNG rng;
	unsigned int i;
	
	_TotalFunctions = nftns;
	_Functions = new Function[ _TotalFunctions ];
	if(_Functions == NULL) { cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
		
	_Edges = new Edge* [_TotalFunctions];
	if( _Edges == NULL)	{ cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
	
	for(i = 0; i < _TotalFunctions; i++)
	{
		_Edges[i] = new Edge[_TotalFunctions];
		if(_Edges[i] == NULL)	{ cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
	}

}

void Application::Init()
{
	RNG rng;
	unsigned int i,j;
	ULL totalExecTime=0;
	ULL execTime;
	
	double* contribArray = new double[ _TotalFunctions ];
	if(contribArray==NULL) { cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
	
	for(i=0;i<_TotalFunctions; i++)
	{
		execTime = ( abs( rng.rand_int31() ) % (execHigh - execLow + 1 ) );
		contribArray[i] = execTime;
		totalExecTime += execTime;
	}
	
	//sort the contribArray in descending order 
	//this is only needed to simplify the initial selection in heuristic
	bsort(contribArray, _TotalFunctions);
	
	for(i=0;i<_TotalFunctions; i++)
	{
		_Functions[i].setExecContrib( contribArray[i] / totalExecTime * 100.0);
	}
	
	double totalcomm=0;
	double maxcomm=0;
	float comm;
	for(i=0 ; i < _TotalFunctions ; i++)
	{
		for(j=0;j< _TotalFunctions;j++)
		{
			//some links will not be there
			comm = ( ( abs( rng.rand_int31() ) % 100 ) < PERCENT_CONNECTIVITY ? 1 : 0 );
			
			//the links which are there now should get random communication
			comm = comm * ( abs( rng.rand_int31() ) % (commHigh - commLow + 1 ) );
			
			_Edges[i][j].setWeight(comm); 
			totalcomm += comm;
			if(comm > maxcomm) maxcomm = comm;
		}
	}	
	
	_MaxComm = maxcomm;
	_TotalComm = totalcomm;
	
	for(i=0 ; i < _TotalFunctions ; i++)
	{
		for(j=0;j< _TotalFunctions;j++)
		{
			//some links will not be there
			_Edges[i][j].setWeight( _Edges[i][j].getWeight()/totalcomm * 100 ); 
		}
	}
	
	delete[] contribArray;
}


void Application::Print()
{
	unsigned int i,j;
	
	cout<<setiosflags(ios::fixed | ios::showpoint);
	cout<<setprecision(2);
	
	cout<<endl<<"Functions"<<endl;
	for(i=0; i< _TotalFunctions; i++)
		_Functions[i].Print();
	
	cout<<endl<<"Edges";

	for(i=0;i < _TotalFunctions;i++)
		cout<<setw(6)<<i;
	
	cout<<endl;
	
	for(i=0;i < _TotalFunctions;i++)
	{
		for(j=0;j< _TotalFunctions;j++)
			if(j==0)
				cout<<setw(6)<<i<<setw(6)<<_Edges[i][j].getWeight();
			else
				cout<<setw(6)<<_Edges[i][j].getWeight();
			
			cout<<endl;
	}
	cout<<endl;
	cout<<resetiosflags(ios::fixed | ios::showpoint);
}

void Application::Print2Dot()
{
	int color;
	unsigned int comm, prod, cons;
	unsigned int i, j;
	
	ofstream dotf;
	dotf.open("graph.dot");
	if (dotf.fail()) {cout<<"\n failed opening the DOT file.\n"; exit(1); }
	
	dotf<<"digraph {"<<endl
		<<"graph [];"<<endl
		<<"node [fontcolor=black, style=filled, fontsize=20];"<<endl
		<<"edge [fontsize=14, arrowhead=vee, arrowsize=0.5];"<<endl;	

	for(i=0; i< _TotalFunctions; i++)
	{
		dotf<<"\""<<i<<"\""<<" [label="<<"\""<<_Functions[i].getFunctionNo()<<"\""<<"];"<<endl;
	}
	
	for(i=0;i < _TotalFunctions;i++)
	{
		for(j=0;j< _TotalFunctions;j++)
		{
			comm = ( _Edges[i][j].getWeight() ) * _TotalComm / 100;
			if(comm == 0 ) continue;
			
			prod = _Functions[i].getFunctionNo();
			cons = _Functions[j].getFunctionNo();
			
			color = (int) (  1023 *  log((double)(comm )) / log((double)_MaxComm)  ); 
			
			dotf<<"\""<<prod<<"\""<<" -> "<<"\""<<cons<<"\""
				<<" [label="<<"\""
				<<comm<<" Bytes "<<"\""
				<<" color= "<<"\""<<"#"
				<<std::noshowbase<<std::hex
 				<<std::setw(2)<<std::setfill('0')
				<<max(0,color-768)
				<<std::setw(2)<<std::setfill('0')				
				<<min(255,512-abs(color-512))
				<<std::setw(2)<<std::setfill('0')				
				<<max(0,min(255,512-color))
				<<"\"]"
 				<<std::dec
				<<endl;
		}
	}
	
	dotf<<"}"<<endl;
	
	dotf.close();
}

void Application::CreateRanks()
{
	for(UINT i =0 ; i<_TotalFunctions;i++)
		_Functions[i].CreateRanks();
}

void Application::Save()
{
	unsigned int i,j;
	ofstream appDataFile;
	string appfname("appData.txt");
	appDataFile.open(appfname.c_str());
	if ( !appDataFile.good() )
	{
		cout<<"Could not open "<<appfname<<"file"<<endl;
		exit(1);
	}
	
	appDataFile<<g_n<<"\t"<<g_k<<endl;
	
	appDataFile<<setiosflags(ios::fixed | ios::showpoint);
	appDataFile<<setprecision(2);
	
	appDataFile<<"Functions"<<endl;
	for(i=0; i< _TotalFunctions; i++)
		appDataFile<<_Functions[i].getExecContrib()<<endl;
	
	appDataFile<<"Edges"<<endl;

	for(i=0;i < _TotalFunctions;i++)
	{
		for(j=0;j< _TotalFunctions;j++)
			appDataFile<<setw(6)<<_Edges[i][j].getWeight();
			
			appDataFile<<endl;
	}
	appDataFile<<endl;
	appDataFile<<resetiosflags(ios::fixed | ios::showpoint);
	
	appDataFile.close();
}

void Application::Restore()
{
	unsigned int i,j;
	float temp;	
	string str;
	ifstream appDataFile;
	string appfname("appData.txt");
	appDataFile.open(appfname.c_str());
	if ( !appDataFile.good() )
	{
		cout<<"Could not open "<<appfname<<"file"<<endl;
		exit(1);
	}
	
	appDataFile>>g_n>>g_k;
	g_applic = new Application(g_n);
	if(g_applic == NULL)
	{
		cout<<"Could not allocate memory for g_applic"<<endl;
		exit(1);
	}
	
	appDataFile>>str;
	cout<<"Restoring "<<str<<"... ";
	for(i=0; i< g_applic->_TotalFunctions; i++)
	{
		appDataFile>>temp;
		g_applic->_Functions[i].setExecContrib(temp);
	}
	cout<<"Done !"<<endl;
	
	appDataFile>>str;
	cout<<"Restoring "<<str<<"... ";
	for(i=0;i<(g_applic->_TotalFunctions);i++)
	{
		for(j=0;j<(g_applic->_TotalFunctions);j++)
		{
			appDataFile>>temp;
			g_applic->_Edges[i][j].setWeight(temp);
		}
	}
	cout<<"Done !"<<endl;
	
	appDataFile.close();
}

