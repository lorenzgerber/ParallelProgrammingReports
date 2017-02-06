#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>


int Read_vector(int block_length, double* vector, double** slice,  int vector_length, int rank, int comm_sz);

int Print_vector(int block_length, double** slice, double* vector, int vector_length, int rank, int comm_sz);

int main(void) {


  int N = 4;
  int my_rank, comm_sz;
  int j;
  
  int matrix[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
  int col[4];
  double vec[18] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
  double readback[18] = { 0 };
  double * slice;


  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Status info;
  
 
    
  // Define type "column"  
  MPI_Datatype column;
  MPI_Type_vector(N,1,N,MPI_INT,&column);
  MPI_Type_commit(&column);



  Read_vector(2, vec, &slice,  18, my_rank, comm_sz);
  Print_vector(2, &slice, readback, 18, my_rank, comm_sz);
  
  if(my_rank==0){
    j=1;
    MPI_Send(&matrix[0][j],1,column,1,99,MPI_COMM_WORLD);
    
  }

  if(my_rank==1){

    MPI_Recv(col,N,MPI_INT,0,99,MPI_COMM_WORLD,&info);
    

    printf("\nColumn: ");

    for(j=0;j<N;j++){
      printf("\n %d",col[j]);
    }
  }
  
  MPI_Type_free(&column);
  MPI_Finalize();
  return 0;
}


int Read_vector(int block_length, double* vector, double** slice, int vector_length, int my_rank, int comm_sz){

  /* calc block cyclic distro */
  int no_cycles = vector_length / block_length * comm_sz;
  int cycle_block_length = block_length * comm_sz;
  
  /* define data type */
  MPI_Datatype block_cyclic;
  MPI_Type_vector(block_length, no_cycles, cycle_block_length, MPI_DOUBLE, &block_cyclic);
  MPI_Type_commit(&block_cyclic);

  
  
  if (my_rank == 0){
    /* loop through all cores to send */
    //for (int i = 1; i < 
    

  } else {

    /* receive */
    

  }

  return 0;
}

  int Print_vector(int block_length, double** slice, double* vector, int vector_length, int my_rank, int comm_sz){

  /* calc block cyclic distro */
  int no_cycles = vector_length / block_length * comm_sz;
  int cycle_block_length = block_length * comm_sz;
  
  /* define data type */
  MPI_Datatype block_cyclic;
  MPI_Type_vector(block_length, no_cycles, cycle_block_length, MPI_DOUBLE, &block_cyclic);
  MPI_Type_commit(&block_cyclic);
    
  if(my_rank == 0){
    printf("test");

  } else {
    printf("test");

  }
  
  return 0;
}
