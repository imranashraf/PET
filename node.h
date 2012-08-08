#ifndef _NODE_H
#define _NODE_H

typedef unsigned long long ULL;
static int count = 0;

class Node
{
	private:
		unsigned int ExecContrib;
		unsigned int NodeNo;

	public:
		Node(){ExecContrib=0; NodeNo=count++;}
		Node(unsigned int contrib)
		{
			ExecContrib = contrib;
			NodeNo=count++;
		}

		void assignContrib(ULL contrib);
		void print();

};

#endif
