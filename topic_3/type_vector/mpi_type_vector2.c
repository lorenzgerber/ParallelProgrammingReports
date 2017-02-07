#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>


int Read_vector(int block_length, double* vector, double** slice,  int vector_length, int rank, int comm_sz);

int Print_vector(int block_length, double** slice, double* vector, int vector_length, int rank, int comm_sz);

int main(void) {
  int my_rank, comm_sz;
  double vec[18] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
  double readback[18] = { 0 };
  double * slice;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  
  Read_vector(2, vec, &slice,  18, my_rank, comm_sz);
  //Print_vector(2, &slice, readback, 18, my_rank, comm_sz);

  /* Print out the result */
  if(my_rank == 0){
    printf("The vector after round Read_vector and Print_vector on Core 0:\n");
    for(int i = 0; i < 18; i++){
      printf("i %d: %f\n", i, readback[i]);
    }
  }

  /* Clean up */
  MPI_Finalize();
  free(slice);
  return 0;
}


int Read_vector(int block_length, double* vector, double** slice, int vector_length, int my_rank, int comm_sz){

  /* calc block cyclic distro */
  int rest_blocks = vector_length % (block_length * comm_sz);
  int no_cycles = vector_length / (block_length * comm_sz);
  int cycle_block_length = block_length * comm_sz;
  int slice_length = no_cycles * block_length;
  int slice_lengths[comm_sz];

  /* populating slice_lengths array */
  for (int i = 0; i < comm_sz; i++){
    int temp_length;
    if ( rest_blocks > 0){
      if (rest_blocks / block_length > i){
	temp_length = slice_length + 1;
      } else {
	temp_length = slice_length;
      }
    } else {
      temp_length = slice_length;
    }
    slice_lengths[i] = temp_length;
  }

  if(my_rank == 0){
    printf("rest_blocks %d\n", rest_blocks);
    printf("slice_length %d\n", slice_length);
    for(int i = 0; i < comm_sz; i++){
      printf("%d \n", slice_lengths[i]);
    }
  }

 
    
  /* define data type */
  MPI_Datatype block_cyclic;
  MPI_Type_vector(1, block_length, cycle_block_length, MPI_DOUBLE, &block_cyclic);
  MPI_Type_commit(&block_cyclic);
  MPI_Status info;
  
  if (my_rank == 0){
    
    for (int i = 1; i < comm_sz;i++){
      MPI_Send(&vector[i*block_length],slice_lengths[i]/block_length,block_cyclic,i,99,MPI_COMM_WORLD);      
    }
    *slice = (double *) calloc(slice_lengths[my_rank], sizeof(double));
    MPI_Sendrecv(&vector[0], slice_lengths[0]/block_length, block_cyclic, 0, 99,
		 *slice, slice_lengths[0], MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &info);
  } else {
    *slice = (double *) calloc(slice_lengths[my_rank], sizeof(double));      
      MPI_Recv(*slice,slice_lengths[my_rank],MPI_DOUBLE, 0, 99, MPI_COMM_WORLD, &info);
  }

  for (int i = 0; i < slice_lengths[my_rank];i++){
    printf("%d/%f ", my_rank, *(*slice+i));
  }
  printf("\n");
    
  

  /* Clean up */
  MPI_Type_free(&block_cyclic);

  return 0;
}


int Print_vector(int block_length, double** slice, double* vector, int vector_length, int my_rank, int comm_sz){

  /* calc block cyclic distro */
  int rest_blocks = vector_length % (block_length * comm_sz);
  int no_cycles = vector_length / (block_length * comm_sz);
  int cycle_block_length = block_length * comm_sz;
  int slice_length = no_cycles * block_length;
  int slice_lengths[comm_sz];

  /* populating slice_lengths array */
  for (int i = 0; i < comm_sz; i++){
    int temp_length;
    if ( rest_blocks > 0){
      if (rest_blocks / block_length > i){
	temp_length = slice_length + 1;
      } else {
	temp_length = slice_length;
      }
    } else {
      temp_length = slice_length;
    }
    slice_lengths[i] = temp_length;
  }


  
  /* calc block cyclic distro */
  /*
  int no_cycles = vector_length / (block_length * comm_sz);
  int cycle_block_length = block_length * comm_sz;
  int slice_length = no_cycles * block_length;
  */
  /* define data type */
  MPI_Datatype block_cyclic;
  MPI_Type_vector(1, block_length, cycle_block_length, MPI_DOUBLE, &block_cyclic);
  MPI_Type_commit(&block_cyclic);
  MPI_Status info;
    
  if(my_rank != 0){
      MPI_Send(*slice,slice_lengths[my_rank],MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);    

  } else {
    for (int i = 1; i < comm_sz;i++){
      MPI_Recv(&vector[i*block_length],slice_lengths[i]/block_length,block_cyclic,i,99,MPI_COMM_WORLD, &info);      
    }
    MPI_Sendrecv(*slice, slice_lengths[0], MPI_DOUBLE, 0, 99, &vector[0], slice_lengths[0]/block_length,
		 block_cyclic, 0, 99, MPI_COMM_WORLD, &info);
  }

  /* Clean up */
  MPI_Type_free(&block_cyclic);
  
  return 0;
}
