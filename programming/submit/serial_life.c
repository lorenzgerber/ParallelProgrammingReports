/*
 * serial_life
 *
 * Coursework 5DV152 Parallel Programming for Multicore based Systems
 * at Umea University, March 2017
 *
 * Lorenz Gerber
 *      
 * Version 0.1
 *
 */

/**
 * @file serial_life.c
 * @author Lorenz Gerber
 * @date 26 March 2017
 * @brief File contains the main method for serial_life
 *
 * serial_life is a serial implementation of conway's game
 * of life. This version was reduced from the parallel MPI
 * version mpi_life.
 */

#include "serial_life.h"

/**
 * @brief Main function
 * serial_life, an implementation of conways game
 * of life.
 */
int main(int argc, char ** argv) {
	int count;
	double start, finish;
	struct life_t life;

	init(&life, &argc, &argv);

	GET_TIME(start);

	for (count = 0; count < life.generations; count++) {

		copy_bounds(&life);
		eval_rules(&life);
		update_grid(&life);
	}
	
	GET_TIME(finish);
	
	printf("%e\n", finish-start);
	
	if (argc == 6)
	  write_grid(&life);
       
	cleanup(&life);

	return 0;
}
