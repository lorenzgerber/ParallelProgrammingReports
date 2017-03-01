/**
 *
 *
 *
 *
 *
 * @usage ./omp_countsort <threads> <data_count>
 */
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <string.h>


void Quick_sort (int a [], int n);
int cmpfunc (const void * a, const void * b);

int main(int argc, char *argv[]){

  int i, j;
  int *test;
  int data_count = strtol(argv[1], NULL, 10);
  double start, finish;

  test = (int*) malloc(data_count*sizeof(int));

  for(j = 0; j < 100; j++){
    
  srand(0);
  for (i = 0; i < data_count; i++)
    test[i] = rand()%101;

  for(int i = 0; i < data_count; i++){
    //printf("%d ", test[i]);
  }
  //printf("\n");

  GET_TIME(start);
  Quick_sort(test, data_count);
  GET_TIME(finish);

  for(int i = 0; i < data_count; i++){
    //printf("%d ", test[i]);
  }

  //printf("\n");

  printf("1 %e\n", (finish - start));

  }

  return 0;
}


void Quick_sort (int a [], int n) {

  qsort(a, n, sizeof(int), cmpfunc);
  
}

int cmpfunc (const void * a, const void * b){
  return ( *(int*)a - *(int*)b);
}
