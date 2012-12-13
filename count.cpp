#include <cstdlib>
#include <cmath>
#include <cassert>
#include <iostream>
#include <climits>
#include "exception.h"

using namespace std;

// long long factorial(long long number)
//  {
//    long long fac = 1;
//    #pragma omp parallel
//    {
//      long long fac_private = 1;
//      #pragma omp for nowait
//      for(long long n=2; n<=number; ++n)
//        fac_private *= n;
//      #pragma omp atomic
//      fac *= fac_private;
//    }
//    return fac;
//  }

long long factorial(long long number)
{
	long long fac = 1;
	for(long long n=2; n<=number; ++n)
		fac = fac * n;
	
	return fac;
}

long long sterling_2nd(int n, int k)
{
    int i; 
    long long comb;
    int parity;

    // sterling number of the second kind
    long long total = 0;
    parity = 1-2*(k%2);
    comb = 1; 
    for(i = 0; i<k+1; i++)
    {
        total += parity*comb*pow(i,n);
        parity = -parity;
        comb = comb*(k-i)/(i+1);
    }
    total = total/factorial(k);

    return total;
}

long long combinations(int n, int k)
{
    long long value = 1;
    long long tmp = 1;
    assert(n>=k);

    if(n < 2*k) k = n - k;

    for(int i = n; i>n-k; i--)
        value *= (long long)i;
    for(int i = 2; i<=k; i++)
        tmp *= (long long)i;

    return value/tmp;
}

long long permutations(int n, int k)
{
    long long value = 1;

    assert(n>=k);
    for(int i = n; i>n-k; i--)
        value *= (long long)i;

    return value;
}

unsigned long long Count(int n, int k)
{
    int x;
	double fact, comb, ster, total = 0;

    for(x=0;x<k;x++)
	{
		fact = factorial(k-x);
		comb = combinations(k,x);
		ster = sterling_2nd(n-k,k-x);
		
		total += fact*comb*ster;
		if(total < 0 || total > ULLONG_MAX )
			throw Exception("Large Number of total partitions",__FILE__,__LINE__);
	}
	
	return total;
}
