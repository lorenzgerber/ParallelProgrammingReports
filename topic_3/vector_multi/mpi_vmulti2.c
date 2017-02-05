#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argv[]) {
  int my_rank, comm_sz;
  int n, local_n, local_dotp_sum = 0, scalar, result_dot;
  int* sendcounts;
  int* displs;
  int* local_vec1;
  int* local_vec2;
  int* vector1;
  int* vector2;

  /* Initializing */
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  srand(time(NULL));

  /* Obtaining Data */
  if(my_rank==0 && argc > 1){

    if(strcmp(argv[1], "r") == 0){
      printf("using random data, vector length = %d\n", 100*comm_sz);
      n = 100*comm_sz;
      vector1 = (int *) malloc(100*comm_sz * sizeof(int));
      vector2 = (int *) malloc(100*comm_sz * sizeof(int));

      for(int i = 0; i < n;i++){
	vector1[i] = rand() % 1000;
	vector2[i] = rand() % 1000;
      }
      scalar = rand() % 1000;
    }
    
  } else if (my_rank==0){
     
    printf("enter vector length\n");
    scanf("%d", &n);
    printf("enter integer vector 1\n");

    vector1 = (int *) malloc(n * sizeof(int));
    vector2 = (int *) malloc(n * sizeof(int));

    for(int i = 0; i < n; i++){
      scanf("%d", &vector1[i]);
    }

    printf("enter integer vector 2\n");

    for(int i = 0; i < n; i++){
      scanf("%d", &vector2[i]);
    }

    printf("enter integer scalar\n");
    scanf("%d", &scalar);
  }

  /* Distribute Data */
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  /* Fixing sendcounts for general n */
  sendcounts = (int *) malloc(comm_sz * sizeof(int));
  displs = (int *) calloc(comm_sz,  sizeof(int));

  for(int i = 0; i < comm_sz;i++){
    if(n % comm_sz > i){
      sendcounts[i] = (n/comm_sz) + 1;
    } else {
      sendcounts[i] = (n/comm_sz);
    }

  }

  for(int i = 1; i <comm_sz; i++){
    displs[i] = displs[i-1]+sendcounts[i];   
  }

  /* show n distribution on ranks*/
  
  if(my_rank==0){
    for(int i = 0; i < comm_sz;i++){
      printf("i sendcounts %d counts %d displs %d\n",i, sendcounts[i], displs[i]);
    }
    printf("------\n");
  }
  
  
 
  local_n = sendcounts[my_rank];

  local_vec1 = (int*) malloc(local_n * sizeof(int));
  local_vec2 = (int*) malloc(local_n * sizeof(int));
  
  MPI_Bcast(&scalar, 1, MPI_INT, 0, MPI_COMM_WORLD);
  //MPI_Scatter(vector1, local_n, MPI_INT, local_vec1, local_n, MPI_INT, 0, MPI_COMM_WORLD);
  //MPI_Scatter(vector2, local_n, MPI_INT, local_vec2, local_n, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Scatterv(vector1, sendcounts, displs, MPI_INT, local_vec1, sendcounts[my_rank], MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatterv(vector2, sendcounts, displs, MPI_INT, local_vec2, sendcounts[my_rank], MPI_INT, 0, MPI_COMM_WORLD);

  /* test prints */
  for(int i = 0; i < comm_sz; i++){
    if(i == my_rank){
      printf("rank %d ", i);
      for(int j = 0; j < local_n; j++){
	printf("%d %d, ", local_vec1[j], local_vec2[j]);
      }
      printf("\n");
    }
  }

  
  /* Calculations */
  /* Calculate Dot Product */
  for(int i = 0;  i< local_n; i++){
    local_vec2[i]*=local_vec1[i];
  }

  /* Calculate vector-scalar product */
  for(int i = 0; i< local_n; i++){
    local_vec1[i]*=scalar;
  }

  /* Summing for dot product */
  for(int i = 0; i < local_n; i++){
    local_dotp_sum += local_vec2[i];    
  } 
 
  /* Collect Data */
  //MPI_Gather(local_vec1, local_n, MPI_INT, vector1, local_n, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Gatherv(local_vec1, sendcounts[my_rank], MPI_INT, vector1, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD); 
  MPI_Reduce(&local_dotp_sum, &result_dot, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  /* Results */
  if(my_rank == 0){
    printf("dot product = %d\n", result_dot);

    printf("vector-scalar product = ");
    for(int i = 0; i < n;i++){
      printf("%d ", vector1[i]);
    }
    printf("\n");
  }

  /* Clean up */
  if(my_rank==0){
    free(vector1);
    free(vector2);
  }
  
  free(local_vec1);
  free(local_vec2);

  MPI_Finalize();

  return 0;
} 


