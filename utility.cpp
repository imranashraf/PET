#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <sstream>

#include "exception.h"
#include "utility.h"
#include "globals.h"

using namespace std;

void Timer::Start()
{
	gettimeofday(&_StartTime, NULL);
}

void Timer::Stop()
{
	gettimeofday(&_StopTime,NULL);
}

void Timer::Print(std::ostream & fout = std::cout)
{
	double t1=_StartTime.tv_sec+(_StartTime.tv_usec/1000000.0);
	double t2=_StopTime.tv_sec+(_StopTime.tv_usec/1000000.0);
	fout<<"Time Elapsed = "<<PrintTime(t2-t1)<<endl;
}

//sorting in descending order
//so Array[0] is the maximum
void bsort(double Array[], int n)
{
	long i, j;
	double temp;
	
	for (i = 0 ; i < ( n - 1 ); i++)
	{
		for (j = 0 ; j < n - i - 1; j++)
		{
			if (Array[j] < Array[j+1])
			{
				/* Swapping */
				
				temp         = Array[j];
				Array[j]   = Array[j+1];
				Array[j+1] = temp;
			}
		}
	}
}		

int Max(double Array[], int n)
{
	int i;
	int index=0;
	
	for(i=1;i<n;i++)
		if(Array[i] > Array[index] )
			index = i;
	
	return index;
}

using namespace std;
std::string PrintTime(long long totalSec)
{
	ostringstream oss;
	long long hr, min, sec;
	
	hr  = floor(totalSec / 3600);
	min = floor((totalSec/60) % 60);
	sec = totalSec % 60;
	
	oss <<"(HH:MM:SS) "
		<<hr <<':'
		<<setfill('0')<<setw(2)<<min<<':'
		<<setfill('0')<<setw(2)<<sec
		<<endl;
	
	return oss.str();
}

std::string EstimateTime(long long partitions)
{
	double totalSec;
	double TimePerPart = 60e-6;				//just an empirical estimate
	totalSec = partitions * TimePerPart;	//total seconds
	return PrintTime(totalSec);
}

std::string int2str(int no)
{
	ostringstream oss;   // stream used for the conversion
	oss<<no;      // insert the textual representation of 'Number' in the characters in the stream
	return oss.str(); 
}

std::string MakeFileName(std::string start, int n, int k, std::string ext)
{
	string fName, tempstr;
	fName = start;
	tempstr = "_";	fName += tempstr;
	fName += int2str(n);
	tempstr = "_";	fName += tempstr;
	fName += int2str(k);
	fName += ext;
	
	return fName;
}

unsigned int get_g_n(std::string fname)
{
	unsigned int n;
	ifstream appDataFile;
	appDataFile.open(fname.c_str());
	if ( appDataFile.fail() )
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}
	appDataFile>>n;
	appDataFile.close();
	
	return n;
}

unsigned int getq2_g_n(std::string maipfname, std::string quadfname)
{
	unsigned long long bytes;
	double contrib;
	unsigned int GlobalfunctionNo=0;
	string str, str1, str2;
	ifstream qfile, mfile;
	
	mfile.open(maipfname.c_str());
	if ( mfile.fail() )
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}
	
	qfile.open(quadfname.c_str());
	if ( qfile.fail() )
	{
		throw Exception("File Opening Error",__FILE__,__LINE__);
	}
	
	while(mfile)
	{
		mfile>>str>>contrib;
		if(!SeenFname.count(str))  // this is the first time I see this function name in charge of access
		{
			SeenFname.insert(str);  // mark this function name as seen
			NametoADD[str]=GlobalfunctionNo;   // create the string -> Number binding
			ADDtoName[GlobalfunctionNo]=str;   // create the Number -> String binding
			GlobalfunctionNo++;      // create a dummy Function Number for this function
		} 
		//cout<<str<<"\t"<<contrib<<endl;
	}
	
	while(qfile)
	{
		qfile>>str1>>str2>>bytes;
		if(!SeenFname.count(str1))  // this is the first time I see this function name in charge of access
		{
			SeenFname.insert(str1);  // mark this function name as seen
			NametoADD[str1]=GlobalfunctionNo;   // create the string -> Number binding
			ADDtoName[GlobalfunctionNo]=str1;   // create the Number -> String binding
			GlobalfunctionNo++;      // create a dummy Function Number for this function
		} 
		
		if(!SeenFname.count(str2))  // this is the first time I see this function name in charge of access
		{
			SeenFname.insert(str2);  // mark this function name as seen
			NametoADD[str2]=GlobalfunctionNo;   // create the string -> Number binding
			ADDtoName[GlobalfunctionNo]=str2;   // create the Number -> String binding
			GlobalfunctionNo++;      // create a dummy Function Number for this function			
		} 
		//cout<<str1<<"\t"<<str2<<"\t"<<bytes<<endl;
	}
	//cout<<"GlobalfunctionNo = "<<GlobalfunctionNo<<endl;
	
	qfile.close();
	mfile.close();
	return GlobalfunctionNo;
}
