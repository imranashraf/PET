#ifndef _NODE_H
#define _NODE_H

typedef unsigned long long ULL;
static int count = 0;

class Function
{
	private:
		unsigned int ExecContrib;
		unsigned int FunctionNo;

	public:
		Function(){ExecContrib=0; FunctionNo=count++;}
		Function(unsigned int contrib)
		{
			ExecContrib = contrib;
			FunctionNo=count++;
		}

		void assignContrib(ULL contrib);
		void print();

};

#endif
