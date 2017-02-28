/**
 * omp_default.c
 * 
 * MP program to determine standard 
 * distribution of threads to for
 * loop indices.
 * @usage ./omp_default <threads> <iterations> 
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]){
  int thread_count, iterations, i, j;
  int *assignments;
  int my_rank;
  int  a[10];

  thread_count = strtol(argv[1], NULL, 10);
  iterations = strtol(argv[2], NULL, 10);

  assignments = (int*) malloc(iterations * sizeof(int));


  a[0] = 0;
# pragma omp parallel for num_threads(thread_count)
  for(i = 1; i < 10; i++){
    a[i] = a[i-1] + i;
  }
  

  # pragma omp parallel for num_threads(thread_count)
  for(i = 0; i < iterations; i++){
    my_rank = omp_get_thread_num();
    assignments[i] = my_rank;
  }

  for(i = 0; i < thread_count; i++){
    printf("Thread %d: Iterations ", i);
    for(j = 0; j < iterations; j++){
      if(assignments[j] == i){
	printf("%d ", j);
      }
    }
    printf("\n");
  }

  return 0;
}
