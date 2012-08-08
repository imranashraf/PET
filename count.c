#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


long long fac(long long number)
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


long long sterling(int n, int k)
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
    total = total/fac(k);

    return total;
}

long long combinations(int n, int k)
{
    return fac(n)/(fac(n-k)*fac(k));
}

long long permutations(int n, int k)
{
    return fac(n)/fac(n-k);
}

long long count(int n, int k)
{
    int x;
    long long total = 0;

    for(x=0;x<k;x++)
        total += fac(k-x)*combinations(k,x)*sterling(n-k,k-x);

    return total;
}


int main(int argc, char *argv[])
{
    int i,j;
    int n = atoi(argv[1]);    
    int k = atoi(argv[2]);

    printf("n:%d k:%d\nsterling:%lld\nnewcount:%lld\nnewcount2:%Lf\n",
        n,k,
        sterling(n,k),
        count(n,k),
        (long double)pow((long double)k,(long double)(n-k))
    );

    return 0;
}
