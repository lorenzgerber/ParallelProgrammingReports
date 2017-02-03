#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int my_rank, comm_sz;
  int n;
  int local_n;
  int* local_vec;
  int* vector1;
  int* vector2;
  int* scalar;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

 

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
      scanf("%d", &vector1[n]);
    }

    printf("enter integer vector 2\n");

    for(int i = 0; i < n; i++){
      scanf("%d", &vector2[n]);
    }

    printf("enter integer scalar\n");
    scanf("%d", scalar);
  }

  //printf("comm_sz = %d", comm_sz);

  

  //local_n = n/comm_sz;
  //local_vec = (int*) malloc(local_n * sizeof(int));
  
  MPI_Bcast(scalar, 1, MPI_INT, 0, MPI_COMM_WORLD);
  //MPI_Scatter(vector1, local_n, MPI_INT, local_vec, local_n, MPI_INT, 0, MPI_COMM_WORLD); 
  

  if(my_rank==0){
    free(vector1);
    free(vector2);
  }

  

  MPI_Finalize();

  return 0;
} 


