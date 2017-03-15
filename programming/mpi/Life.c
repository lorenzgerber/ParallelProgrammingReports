/*******************************************
MPI Life 1.0
Copyright 2002, David Joiner and
  The Shodor Education Foundation, Inc.
Updated 2010, Andrew Fitz Gibbon and
  The Shodor Education Foundation, Inc.

A C implementation of Conway's Game of Life.

To run:
./Life [Rows] [Columns] [Generations] [Display]

See the README included in this directory for
more detailed information.
*******************************************/

#include "Life.h"
#include "Defaults.h" // For Life's constants

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

		///throttle(&life);
	}
	
	GET_TIME(finish);
	
	printf("%e\n", finish-start);
       
	cleanup(&life);

	exit(EXIT_SUCCESS);
}
