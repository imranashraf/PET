#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <iostream>

typedef unsigned long long ULL;
typedef unsigned int UINT;

static int count = 0;

class Function
{
	private:
		float _ExecContrib;
		UINT _FunctionNo;
		UINT _ClusterNo;	//no of the cluster to which this function belongs
							//-1 for not assigned yet
		double * _Ranks;

	public:
		Function(){_ExecContrib=0; _FunctionNo=count++; _ClusterNo=-1;}
		Function(float contrib)
		{
			_ExecContrib = contrib;
			_FunctionNo=count++;
			_ClusterNo=-1;
		}

		void setExecContrib(float contrib);
		float getExecContrib(){return _ExecContrib;}
		UINT getFunctionNo(){return _FunctionNo;}
		
		void CreateRanks();
		
		void setClusterNo(UINT cno){_ClusterNo = cno;}
		UINT getClusterNo(){return _ClusterNo;}
		
		void Init()
		{
			_ClusterNo=-1;
		}
		
		void print();
};

#endif
