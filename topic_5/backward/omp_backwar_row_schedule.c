#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <omp.h>
#include <string.h>


int main(int argc, char *argv[]){

  double** A;
  double* b;
  double* x;
  int thread_count, row, col, i, j;
  int n = strtol(argv[2], NULL, 10);
  double start, finish;

  thread_count = strtol(argv[1], NULL, 10);

  b = malloc(n * sizeof(double));
  x = malloc(n * sizeof(double));


  
  
  // allocate 2d int array size n * n
  A = malloc(n * sizeof(double*));
  for (int i = 0; i < n; i++) {
    A[i] = malloc(n * sizeof(double));
  }

  for(int p = 2; p < 1025; p*=2){
    //for(int p = 10; p < 1001; p*=10){
  for(int k = 0; k < 10;k++){
    
 
  srand(0);
  for (i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      A[i][j] = i+j;
    }
  }

  for (i = 0; i < n; i++){
    b[i] = i;
  }

  //thread_count = strtol(argv[1], NULL, 10);


  GET_TIME(start);
  // backward substitution, column wise


  for (row = 0; row < n; row++)
    x[row] = b[row];

#pragma omp parallel
  {
  for (col = n-1; col >= 0; col--){
#   pragma omp single
    x[col] /= A[col][col];

# pragma omp for
    for (row = 0; row < col; row++)
      x[row] -= A[row][col]*x[col];
  }
  } 

  GET_TIME(finish);

  //for(i=0; i< n; i++){
  //  printf("%f ", x[i]);
  //}
  //printf("\n");
  printf("%d %e\n", p, (finish-start));

  }
  }

  for (i = 0; i < n; i++) {
    free(A[i]);
  }
  free(A);

  return 0;
}
