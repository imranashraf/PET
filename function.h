#ifndef _NODE_H
#define _NODE_H

typedef unsigned long long ULL;
static int count = 0;

class Function
{
	private:
		float _ExecContrib;
		unsigned int _FunctionNo;

	public:
		Function(){_ExecContrib=0; _FunctionNo=count++;}
		Function(float contrib)
		{
			_ExecContrib = contrib;
			_FunctionNo=count++;
		}

		void setExecContrib(float contrib);
		float getExecContrib(){return _ExecContrib;}
		void print();

};

#endif
