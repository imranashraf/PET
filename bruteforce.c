#include <stdlib.h>
#include <stdio.h>

typedef void (*eval_func) (int n, int k, int *partition);

static eval_func evaluate;
static int *partition;
static int g_n, g_k;
static void _bruteforce(int n)
{
    int i;
    // pos n iterate over k -> recursively call _bruteforce with n-1 and k
    if(n==0)
    {
        evaluate(g_n,g_k,partition);
    }
    else
    {
        for(i=0; i<g_k; i++)
        {
            partition[n-1] = i; 
            _bruteforce(n-1);
        }
    }
}

void bruteforce(int n, int k, eval_func eval)
{
    int i;

    g_n = n;
    g_k = k;
    evaluate = eval;
    partition = (int *)malloc(sizeof(int)*n);

    for(i=0;i<n;i++)
        partition[i] = 0;

    _bruteforce(n);

    free(partition);
}

void bruteforce_kfixed(int n, int k, eval_func eval)
{
    int i;

    g_n = n;
    g_k = k;
    evaluate = eval;
    partition = (int *)malloc(sizeof(int)*n);

    for(i=0;i<n-k;i++)
        partition[i] = 0;
    for(;i<n;i++)
        partition[i] = i+k-n;

    _bruteforce(n-k);

    free(partition);
}

void _bruteforce_nonempty(int n, int k)
{
    int i;
    if(n == k) {
        evaluate(g_n,g_k,partition);
    } else {
        for(i=0; i < k; i++)
        {
            partition[n - 1] = i;
            _bruteforce_nonempty(n-1,k);
        }
        partition[n - 1] = n - 1;
        if(k>1)
        {
            partition[n - 1] = k - 1;
            _bruteforce_nonempty(n - 1, k - 1);
            partition[n - 1] = n - 1;
        }
    }
}

void bruteforce_nonempty(int n, int k, eval_func eval)
{
    int i;

    g_n = n;
    g_k = k;
    evaluate = eval;
    partition = (int *)malloc(sizeof(int)*n);

    for(i=0;i<n;i++)
        partition[i] = 0;

    _bruteforce_nonempty(n,k);

    free(partition);
}


#ifdef TESTMODE
void print_part (int n, int k, int *partition)
{
    while(n--) 
        printf("%d ",partition[n]);
    printf("\n");
}

int main(int argc, char *argv[])
{
    int n,k,t;

    if(argc!=3 && argc!=4) {
        printf("syntax: %s n k <t>\n\tn,k: integer\n\t<t>: optional boolean -> k fixed cluster kernels", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    k = atoi(argv[2]);
    if(argc==4) t = atoi(argv[3]);
    else t = 0;
   
    if(t==0)
        bruteforce(n,k,print_part);
    else if(t==1)
        bruteforce_kfixed(n,k,print_part);
    else if(t==2)
        bruteforce_nonempty(n,k,print_part);
    
    return 0; 
}
#endif
