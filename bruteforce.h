#ifndef _BRUTEFORCE_H
#define _BRUTEFORCE_H

typedef void (*eval_func) (int n, int k, int *partition);

void bruteforce(int n, int k, eval_func eval);
void bruteforce_kfixed(int n, int k, eval_func eval);
void bruteforce_nonempty(int n, int k, eval_func eval);

void print_part (int n, int k, int *partition);

#endif