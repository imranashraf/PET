#ifndef _CLUSTER_H
#define _CLUSTER_H

class Cluster
{
	private:
		unsigned int _FunctionCount;
		unsigned int _FunctionCapacity;
		unsigned int * _Functions;
	
	public:
		Cluster(){_FunctionCount=0;}
		Cluster(unsigned int totalFunctions);
		void setFunctionCapacity(unsigned int totalFunctions);
		void addFunction(unsigned int nodeNo);
		float ExecCost();
		float ExternalComm();
		float InternalComm();
		bool inCluster(unsigned int fno);
		
		void Print();
};

#endif

