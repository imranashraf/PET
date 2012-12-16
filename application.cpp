#include <iostream>
#include <iomanip>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <list>

#include "application.h"
#include "rng.h"
#include "utility.h"
#include "globals.h"
#include "exception.h"

using namespace std;

Application::Application(unsigned int nftns)
{
	RNG rng;
	unsigned int i;
	
	_TotalFunctions = nftns;
	
	try
	{
		_Functions = new Function[ _TotalFunctions ];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	for(i=0;i<_TotalFunctions;i++)
		_Functions[i].setFunctionNo(i);
	
	try
	{
		_Edges = new Edge* [_TotalFunctions];
		for(i = 0; i < _TotalFunctions; i++)
		{
			_Edges[i] = new Edge[_TotalFunctions];
			if(_Edges[i] == NULL)	{ cout<<"\n Memory Allocation Error "<<endl;  exit(1); }
		}
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
}

void Application::Init()
{
	RNG rng;
	unsigned int i,j;
	ULL totalExecTime=0;
	ULL execTime;
	double* contribArray;
	
	try
	{
		contribArray = new double[ _TotalFunctions ];
	}
	catch (const std::bad_alloc& e) 
	{
		cout<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
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


void Application::Print(std::ostream & fout = std::cout)
{
	unsigned int i,j;
	int defaultPrecision = cout.precision();
	fout<<setiosflags(ios::fixed | ios::showpoint);
	fout<<setprecision(2);
	
	fout<<endl<<"Functions"<<endl;
	for(i=0; i< _TotalFunctions; i++)
	{
		_Functions[i].Print(fout);
		fout<<endl;
	}
	
	fout<<endl<<"Edges";

	for(i=0;i < _TotalFunctions;i++)
		fout<<setw(6)<<i;
	
	fout<<endl;
	
	for(i=0;i < _TotalFunctions;i++)
	{
		for(j=0;j< _TotalFunctions;j++)
			if(j==0)
				fout<<setw(6)<<i<<setw(6)<<_Edges[i][j].getWeight();
			else
				fout<<setw(6)<<_Edges[i][j].getWeight();
			
			fout<<endl;
	}
	fout<<endl;
	fout<<resetiosflags(ios::floatfield | ios::showpoint );
	fout.precision(defaultPrecision);
	fout<<fixed;
}

void Application::Print2Dot()
{
	int color;
	unsigned int comm, prod, cons;
	unsigned int i, j;
	ofstream dotf;
	string dotFile, tempstr;
	
	dotFile = "graph_";
	dotFile += int2str(g_n);
	tempstr = "_";	dotFile += tempstr;
	dotFile += int2str(g_k);
	tempstr = ".dot";	dotFile += tempstr;
	dotf.open(dotFile.c_str());
	if (dotf.fail()) 
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}
	
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

void Application::Save()
{
	unsigned int i,j;
	ofstream appDataFile;
	string appfname("appData.txt");
	appDataFile.open(appfname.c_str());
	if ( appDataFile.fail() )
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}
	
	appDataFile<<g_n<<endl;
	
	appDataFile<<setiosflags(ios::fixed | ios::showpoint);
	appDataFile<<setprecision(2);
	int defaultPrecision = appDataFile.precision();
	
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
	appDataFile.precision(defaultPrecision);
	
	appDataFile.close();
}

void Application::Restore()
{
	unsigned int i,j;
	float temp;
	unsigned int tempi;
	string str;
	ifstream appDataFile;
	string appfname("appData.txt");
	appDataFile.open(appfname.c_str());
	if ( appDataFile.fail() )
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}
	
	appDataFile>>tempi; //is n

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

void Application::RestoreQ2()
{
	unsigned int i,j;
	unsigned long long bytes;
	double contrib;
	string str, str1, str2;
	ifstream qfile, mfile;
	qfile.open("quadData.txt");
	if ( qfile.fail() )
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}
	
	mfile.open("maipData.txt");
	if ( mfile.fail() )
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}
	
	//read contributions from maipData file (values are already sorted,
	//	a simplifying requirement for initial selection)
	for(i=0; i< _TotalFunctions; i++)
	{
		_Functions[i].setExecContrib(0.0);
	}
	
	while(mfile)
	{
		mfile>>str>>contrib;
		int fno = NametoADD[str];
		_Functions[fno].setExecContrib(contrib);
	}
	
	//read communication values from quadData file
	double totalcomm=0;
	double maxcomm=0;
	
	//initialize communication to 0.0 for the links which are not there
	for(i=0 ; i < _TotalFunctions ; i++)
	{
		for(j=0;j< _TotalFunctions;j++)
		{
			_Edges[i][j].setWeight(0.0); 
		}
	}	

	//assign communication and also calculate total and maximum
	while(qfile)
	{
		qfile>>str1>>str2>>bytes;
		unsigned int pfno = NametoADD[str1];
		unsigned int cfno = NametoADD[str2];
		_Edges[pfno][cfno].setWeight(bytes); 
		totalcomm += bytes;
		if(bytes > maxcomm) maxcomm = bytes;
	}
	
	_MaxComm = maxcomm;
	_TotalComm = totalcomm;
	
	//change to percentages
	for(i=0 ; i < _TotalFunctions ; i++)
	{
		for(j=0;j< _TotalFunctions;j++)
		{
			_Edges[i][j].setWeight( _Edges[i][j].getWeight()/totalcomm * 100 ); 
	
		}
	}
	
	qfile.close();
	mfile.close();
}

class MyData
{
	public:
		int edgeNo;
		double edgeWeight;
		bool operator<(MyData rhs) { return edgeWeight > rhs.edgeWeight; }
};

void Application::Filter()
{
	unsigned int nContribThresh = EXEC_THRESHOLD;
	unsigned int nCommThresh = COMM_THRESHOLD;
	unsigned int nTotalThresh = nContribThresh + nCommThresh;	//these are the total number of functions in the filtered application
	TotalFilteredComm=0, TotalFilteredContrib=0;
	
	set<unsigned int>filteredFtnNos;
	list<MyData> edgeList;
	
	if(nContribThresh > g_n)
		throw Exception("nContribThresh should be less than n",__FILE__,__LINE__);
	if(nCommThresh > g_n )
		throw Exception("nCommThresh should be less than n",__FILE__,__LINE__);
	if(nTotalThresh > g_n )
		throw Exception("nTotalThresh should be less than n",__FILE__,__LINE__);
	
	MyData data;
	unsigned int e=0, i,j;
	for(i=0 ; i < _TotalFunctions ; i++)
	{
		for(j=0;j< _TotalFunctions;j++)
		{
			data.edgeNo = e++;
			data.edgeWeight = _Edges[i][j].getWeight();
			edgeList.push_back(data);
		}
	}
	
	edgeList.sort();
	
	//make set of filtered function numbers
	unsigned int f, edge, prod, cons;
	for(f=0; f<nContribThresh; f++)
	{
		filteredFtnNos.insert(f);	//top nContribThresh ftns filtered (as they are already in ascending order)
	}
	for (list<MyData>::const_iterator citer = edgeList.begin(); (citer != edgeList.end()) ; ++citer)
	{
		edge = (*citer).edgeNo;
		prod = edge / g_n;	//row
		cons = edge % g_n;	//col
		if (f<nTotalThresh && filteredFtnNos.count(prod)==0)	{ filteredFtnNos.insert(prod); f++; }
		if (f<nTotalThresh && filteredFtnNos.count(cons)==0)	{ filteredFtnNos.insert(cons); f++; }
		
		if(f>=nTotalThresh) break;
	}
	
	//create the filtered application
	try
	{
		g_filtered_applic = new Application(nTotalThresh);
	}
	catch (const std::bad_alloc& e) 
	{
		cerr<<e.what()<<endl;
		throw Exception("Allocation Failed",__FILE__,__LINE__);
	}
	
	//set the execution contributions of functions in new application
	double contrib;
	set<unsigned int>::iterator it;
	for ( i=0 , it=filteredFtnNos.begin() ; it != filteredFtnNos.end(); i++ , it++ )
	{
		contrib = g_applic->getFunctionContrib(*it);
  		g_filtered_applic->setFunctionContrib(i,contrib);
		TotalFilteredContrib+=contrib;
	}
	
	//set the communication percentage of edges of new application
	double weight;
	set<unsigned int>::iterator it1, it2;
	for ( i=0, it1=filteredFtnNos.begin(); it1 != filteredFtnNos.end(); i++, it1++ )
	{
		for ( j=0, it2=filteredFtnNos.begin(); it2 != filteredFtnNos.end(); j++, it2++ )
		{
			weight = g_applic->getEdgeWeight(*it1,*it2);
			g_filtered_applic->setEdgeWeight(i,j,weight);
			TotalFilteredComm+=weight;
		}
	}
	
}
