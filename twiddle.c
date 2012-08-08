#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

static int *a;
static int *b;
static int g_n;
static time_t ts;
static long long total = 0;

static int *costs;



int partition_cost()
{
    int i,j,index=0;
    int cost = 0;
//    #pragma omp parallel for reduction(+: cost)
    for(j=0; j<g_n;j++)
        for(i=0; i<g_n;i++,index++) {
                cost += (a[i]!=a[j])*costs[index];
        }
    return cost;
}

void print_partition(int *part, int n)
{
    int i;
    for(i=0; i < n; i++)
    {
        printf("%d ", part[i]);
    }
    printf("\n");
}

static int best_sofar = INT_MAX;

static long long seq_nr = 0;

void _subsets(int n, int nc)
{
    int i;
    if(n == nc) {
        int cost = partition_cost();
        if(cost<best_sofar)
        {
            best_sofar = cost;
            memcpy(b,a,sizeof(int)*n);
        }
        if((seq_nr++%266144)==0) {
            time_t ts1 = time(0);
            int tdiff = ts1-ts;
            int eta = (tdiff*total)/seq_nr;
            printf("%3d %% completed (%d:%02d/%d:%02d)\r",(int)(seq_nr*100/total),tdiff/60,tdiff%60,eta/60,eta%60);
            fflush(stdout);
        }
        //print_partition(a,g_n);
    } else {
        for(i=0; i < nc; i++)
        {
            a[n - 1] = i;
            _subsets(n-1,nc);
        }
        a[n - 1] = n - 1;
        if(nc>1)
        {
            a[n - 1] = nc - 1;
            _subsets(n - 1, nc - 1);
            a[n - 1] = n - 1;
        }
    }
}

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


void subsets(int n, int nc)
{
    int i; 
    long long comb;
    int parity;

    a = (int*)calloc(sizeof(int),n);
    g_n = n;

    for(i=0; i<n;i++) 
        a[i] = i;

    // sterling number of the second kind
    total = 0;
    parity = 1-2*(nc%2);
    comb = 1; 
    for(i = 0; i<nc+1; i++)
    {
        total += parity*comb*pow(i,n);
        parity = -parity;
        comb = comb*(nc-i)/(i+1);
    }
    total = total/fac(nc);

    printf("total number of partitions: %lld\n", total);
    printf("%3d %% completed\r",0);

    ts = time(0);
    _subsets(n,nc);


    free(a);
}

int main(int argc, char *argv[])
{
    int i,j;
    int n = atoi(argv[1]);    
    int nc = atoi(argv[2]);

    srand(time(0));

    costs = calloc(sizeof(int),n*n);

    for(j=0; j<n;j++) {
        for(i=0; i<n;i++) {
            costs[i+j*n] = rand()%5;
            printf("%d ", costs[i+j*n]);
        }
        putchar('\n');
    }
    

    b = (int*)calloc(sizeof(int),n);
    subsets(n,nc);

    printf("lowest possible cost: %d\n", best_sofar);

    print_partition(b,n);
    
    free(costs);
    free(b);
}
