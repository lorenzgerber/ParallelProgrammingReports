#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <omp.h>
#include <string.h>


int main(int argc, char *argv[]){

  double a;
  double* x;
  double* y;
  int i;
  int thread_count = strtol(argv[1], NULL, 10);
  int n = strtol(argv[2], NULL, 10);
  int c = strtol(argv[3], NULL, 10);
  
  double start, finish;

  x = malloc(n * sizeof(double));
  y = malloc(n * sizeof(double));
  
  srand(0);
  a = rand();
  for (i = 0; i < n; i++){
    x[i] = rand();
    y[i] = rand();
  }
  
  GET_TIME(start);

# pragma omp parallel private(i) num_threads(thread_count)
  {

#   pragma omp for schedule(static, c)
    for (i = 0; i < n; i++)
      x[i]=a*x[i];
#   pragma omp for schedule(static, c)
    for(i = 0; i < n; i++)
      y[i] = x[i] + y[i];
  }

  GET_TIME(finish);

  printf("%d %d %e\n", thread_count, c, (finish-start));

  free(x);
  free(y);

  return 0;
}
