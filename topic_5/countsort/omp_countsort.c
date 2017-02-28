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
#ifdef _OPENMP
#include <omp.h>
#endif
#include <string.h>


void Count_sort (int a [], int n , int thread_count);

int main(int argc, char *argv[]){

  int thread_count, i;
  int *test;
  int data_count = strtol(argv[2], NULL, 10); 

  test = (int*) malloc(data_count*sizeof(int));
    
  srand(0);
  for (i = 0; i < data_count; i++)
    test[i] = rand()%101;

  thread_count = strtol(argv[1], NULL, 10);

  for(int i = 0; i < data_count; i++){
    printf("%d ", test[i]);
  }
  printf("\n");

  Count_sort(test, data_count, thread_count);

  for(int i = 0; i < data_count; i++){
    printf("%d ", test[i]);
  }

  printf("\n");
  

  return 0;
}


void Count_sort (int a [], int n , int thread_count) {
  int i, j , count;
  //int my_rank = omp_get_thread_num();
  
  int* temp = malloc ( n *sizeof(int));

# pragma omp parallel for num_threads(thread_count) \
  private(j, count)
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
