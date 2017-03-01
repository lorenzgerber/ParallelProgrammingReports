#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <string.h>


int main(int argc, char *argv[]){

  double** A;
  double* b;
  double* x;
  int thread_count, row, col, i, j;
  int n = strtol(argv[1], NULL, 10);
  double start, finish;

  b = malloc(n * sizeof(double));
  x = malloc(n * sizeof(double));

  // allocate 2d int array size n * n
  A = malloc(n * sizeof(double*));
  for (int i = 0; i < n; i++) {
    A[i] = malloc(n * sizeof(double));
  }

  srand(0);
  for (i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      A[i][j] = rand()%21-10;
    }
  }

  for (i = 0; i < n; i++){
    b[i] = rand()%21-10;
  }

  //thread_count = strtol(argv[1], NULL, 10);


  for (row = n-1; row >= 0; row--) { x[row] = b[row];
    for (col = row+1; col < n; col++)
      x[row] -= A[row][col]*x[col]; x[row] /= A[row][row];
  }


  for (i = 0; i < n; i++) {
    free(A[i]);
  }
  free(A);

  return 0;
}
