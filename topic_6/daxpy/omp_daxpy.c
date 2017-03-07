#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <omp.h>
#include <string.h>


int main(int argc, char *argv[]){

  double a;
  double* b;
  double* x;
  int thread_count, i;
  int n = strtol(argv[2], NULL, 10);
  double start, finish;

  thread_count = strtol(argv[1], NULL, 10);

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

#   pragma omp for schedule(static,1)
    for (i = 0; i < n; i++)
      x[i]=a*x[i];
#   pragma omp for schedule(static,1)
    for(i = 0; i < n; i++)
      y[i] = x[i] + y[i];
  }

  GET_TIME(finish);

  printf("%e\n", (finish-start));

  free(x);
  free(y);

  return 0;
}
