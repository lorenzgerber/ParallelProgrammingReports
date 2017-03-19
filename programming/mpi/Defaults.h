/*******************************************
MPI Life 1.0
Copyright 2002, David Joiner and
  The Shodor Education Foundation, Inc.
Updated 2010, Andrew Fitz Gibbon and
  The Shodor Education Foundation, Inc.
*******************************************/

#ifndef BCCD_LIFE_DEFAULTS_H
#define BCCD_LIFE_DEFAULTS_H

#include <stdbool.h>
#include <getopt.h>
#include <stddef.h>

// Default parameters for the simulation
const int     DEFAULT_SIZE = 105;
const int     DEFAULT_GENS = 10000;
const double     INIT_PROB = 0.25;

// All the data needed by an instance of Life
struct life_t {
	int  rank;
	int  size;
	int  throttle;
	int  ncols;
	int  nrows;
	int  ** grid;
	int  ** next_grid;
	bool do_display;
	int  generations;
	char * infile;
	char * outfile;

	struct display_t disp;
};

enum CELL_STATES {
	DEAD = 0,
	ALIVE
};

// Cells become DEAD with more than UPPER_THRESH 
// or fewer than LOWER_THRESH neighbors
const int UPPER_THRESH = 3;
const int LOWER_THRESH = 2;

// Cells with exactly SPAWN_THRESH neighbors become ALIVE
const int SPAWN_THRESH = 3;

#endif
