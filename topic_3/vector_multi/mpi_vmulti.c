#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int my_rank, comm_sz;
  int n;
  int local_n;
  int local_dotp_sum = 0;
  int* local_vec1;
  int* local_vec2;
  int* vector1;
  int* vector2;
  int scalar;
  int result_dot;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);


  //n = (int *) malloc(sizeof(int));
  //scalar = (int *) malloc(sizeof(scalar));

  if(my_rank==0){
    if(argc > 1){
      if(strcmp(argv[1], "r") == 0){
	printf("random\n");
	// Here comes the random generator
      }
    }

    printf("enter vector length\n");
    
    scanf("%d", &n);
    printf("vector of length %d is assigned\n", n);


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
  
  local_n = n/comm_sz;
  local_vec1 = (int*) malloc(local_n * sizeof(int));
  local_vec2 = (int*) malloc(local_n * sizeof(int));

  
  MPI_Bcast(&scalar, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(vector1, local_n, MPI_INT, local_vec1, local_n, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(vector2, local_n, MPI_INT, local_vec2, local_n, MPI_INT, 0, MPI_COMM_WORLD);

  printf("my_rank = %d, local_n = %d\n", my_rank, local_n);

  for(int i = 0; i < local_n; i++){
    printf("\n%d = %d, %d\n", i, local_vec1[i], local_vec2[i]);
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
  MPI_Gather(local_vec1, local_n, MPI_INT, vector1, local_n, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Reduce(&local_dotp_sum, &result_dot, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  /* Results */

  if(my_rank == 0){
    printf("dot product = %d\n", result_dot);

    printf("vector-scalar product = ");
    for(int i = 0; i < n;i++){
      printf("%d ", vector1[i]);
    }
    
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


