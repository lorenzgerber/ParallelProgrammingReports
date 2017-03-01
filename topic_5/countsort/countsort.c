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


void Count_sort (int a [], int n);

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
  Count_sort(test, data_count);
  GET_TIME(finish);

  for(int i = 0; i < data_count; i++){
    //printf("%d ", test[i]);
  }

  //printf("\n");

  printf("%e\n", (finish - start));

  }

  return 0;
}


void Count_sort (int a [], int n) {
  int i, j , count;
  
  int* temp = malloc ( n *sizeof(int));
  
  for ( i = 0; i < n ; i ++) {
    count = 0;
    for ( j = 0; j < n ; j ++)
      if ( a[j] < a[i])
	count ++;
      else if (a[j] == a[i] && j < i )
	count ++;
    temp[count] = a [i];
  }
  
  memcpy ( a , temp , n *sizeof(int));
  free ( temp );
}
