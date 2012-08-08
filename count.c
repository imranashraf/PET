#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


long long factorial(long long number)
 {
   long long fac = 1;
   #pragma omp parallel
   {
     long long fac_private = 1; /* This value comes from the table shown above */
     #pragma omp for nowait
     for(long long n=2; n<=number; ++n)
       fac_private *= n;
     #pragma omp atomic
     fac *= fac_private;
   }
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

/*long long count(int n, int k)
{
    int x;
    long long total = 0;

    for(x=0;x<k;x++)
        total += factorial(k-x)*combinations(k,x)*sterling(n-k,k-x);

    return total;
}*/
