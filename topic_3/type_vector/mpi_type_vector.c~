#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(void) {

  int N = 4;
  int myrank, nprocs;
  int i,j;

  int matrix[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
  int col[4];

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Status info;


    // Define type "column"
    MPI_Datatype column;
    MPI_Type_vector(N,1,N,MPI_INT,&column);
    MPI_Type_commit(&column);

    if(myrank==0){
      j=1;
      MPI_Send(&matrix[0][j],1,column,1,99,MPI_COMM_WORLD);
    }

    if(myrank==1){
      // **** FIRST MODE: Don't use "column" type *****
      MPI_Recv(col,N,MPI_INT,0,99,MPI_COMM_WORLD,&info);

      // **** SECOND MODE: Use "column" type *****
      // MPI_Recv(col,1,column_INT,0,99,MPI_COMM_WORLD,&info);

      printf("\nColumn: ");
      for(j=0;j<N;j++)
	printf("\n %d",col[j]);
    }

    MPI_Type_free(&column);
    MPI_Finalize();

    return 0;
}
