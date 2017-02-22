#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

/* Global variables */
int     thread_count = 1;
void *Pth_empty(void* rank);
int main(int argc, char* argv[]) {
  
  long       thread;
  pthread_t* thread_handles;
  double start, finish;


  for(int j = thread_count; j <= 8; j=j+2){
    

  for (int i = 0; i < 100; i++){
    GET_TIME(start);
    thread_handles = malloc(j*sizeof(pthread_t));

    for (thread = 0; thread < j; thread++){
      pthread_create(&thread_handles[thread], NULL,
		     Pth_empty, (void*) thread);
    }

    for (thread = 0; thread < j; thread++){
      pthread_join(thread_handles[thread], NULL);
    }

    GET_TIME(finish);
    free(thread_handles);

    printf("%d %e\n", j, (finish - start)/j);
  }
  }

  return 0;
}

void *Pth_empty(void* rank){
  return NULL;
}
