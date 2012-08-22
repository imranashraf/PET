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
		double * _Ranks;

	public:
		Function(){_ExecContrib=0; _FunctionNo=count++;}
		Function(float contrib)
		{
			_ExecContrib = contrib;
			_FunctionNo=count++;
		}

		void setExecContrib(float contrib);
		float getExecContrib(){return _ExecContrib;}
		UINT getFunctionNo(){return _FunctionNo;}
		void CreateRanks(UINT k)
		{
			_Ranks = new double[k];
			if(_Ranks==NULL)
				std::cout<<"Could not allocate memory for Ranks "<<std::endl;
		}
		
		void print();
};

#endif
