/*
 * mpi_life
 *
 * Coursework 5DV152 Parallel Programming for Multicore based Systems
 * at Umea University, March 2017
 *
 * Lorenz Gerber
 *
 * Version 0.1
 *
 * Licensed under GPLv3
 *
 */

/**
 * @file mpi_life.c
 * @author Lorenz Gerber
 * @date 26 March 2017
 * @brief File contains the main method for mpi_life
 *
 * mpi_life is a mpi implementation of conway's game
 * of life. Certain parts of the code are inspired
 * by Davind Joiner's implementation found on shodor.org.
 */
#include "mpi_life.h"


/**
 * @brief Main function
 *
 * This is the main function to be started for
 * mpi_life, an implementation of conways game
 * of life. 
 */
int main(int argc, char ** argv) {
  int count;
  double start, finish;
  struct life_t life;

  init(&life, &argc, &argv);

  MPI_Barrier(MPI_COMM_WORLD);
  start = MPI_Wtime();
  
  for (count = 0; count < life.generations; count++) {

    copy_bounds(&life);
    eval_rules(&life);
    update_grid(&life);
}

  finish = MPI_Wtime();
  if(life.rank == 0){
    printf("%e\n", finish-start);
  }

  cleanup(&life);
  return 0;
}
