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
 * Licensed under GPLv3
 */

/**
 * @file serial_life.h
 * @author Lorenz Gerber
 * @date 26 March 2017
 * @brief File contains the main method for serial_life
 *
 * serial_life is a serial implementation of conway's game
 * of life. This version was reduced from the parallel MPI
 * version mpi_life.
 */

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "timer.h"


/**
 * @brief struct life_t holds old needed variables for
 * computation of the game.
 */
struct life_t {
  int  rank;
  int  ncols;
  int  nrows;
  int  ** grid;
  int  ** next_grid;
  int  generations;
  char * outfile;
  char * infile;
};

/**
 * @brief enum on the cell states. 
 * ALIVE are all values larger than zero.
 */
enum CELL_STATES {
	DEAD = 0,
	ALIVE
};

const double INIT_PROB = 0.25;
const int UPPER_THRESH = 3;
const int LOWER_THRESH = 2;
const int SPAWN_THRESH = 3;

int               init (struct life_t * life, int * c, char *** v);
void        eval_rules (struct life_t * life);
void       copy_bounds (struct life_t * life);
void       update_grid (struct life_t * life);
void    allocate_grids (struct life_t * life);
void        init_grids (struct life_t * life);
void        free_grids (struct life_t * life);
double     rand_double ();
void    randomize_grid (struct life_t * life, double prob);
void       seed_random (int rank);
void           cleanup (struct life_t * life);
void             usage ();

/**
 * @brief Initilizes the game
 *
 * Initializes the main data container struct life,
 * parses the command line args and finally sets
 * up the game field.
 */
int init (struct life_t * life, int * c, char *** v) {
  int argc          = *c;
  char ** argv      = *v;
  life->ncols       = 0;
  life->nrows       = 0;
  life->generations = 0;
  life->outfile     = NULL;
  life->infile      = NULL;

  if (argc >= 4){
    life->ncols = strtol(argv[1], (char**) NULL, 10);
    life->nrows = strtol(argv[1], (char**) NULL, 10);
    life->generations = strtol(argv[2], (char**) NULL, 10);
    life->outfile   = argv[3];
    if(argc == 5){
      life->infile = argv[4];
    }
  } else {
       printf("usage: serial_life n_size generations outfile [infile]");
       exit(1);
    }

  seed_random(life->rank);
  init_grids(life);

  return 0;

}

/**
 * @brief game step that determine the number
 *
 * of neighbours in the temporary grid.
 */
void eval_rules (struct life_t * life) {
  int i,j,k,l,neighbors;

  int ncols = life->ncols;
  int nrows = life->nrows;

  int ** grid      = life->grid;
  int ** next_grid = life->next_grid;

  for (i = 1; i <= ncols; i++) {
    for (j = 1; j <= nrows; j++) {
      neighbors = 0;

      // count neighbors
      for (k = i-1; k <= i+1; k++) {
	for (l = j-1; l <= j+1; l++) {
	  if (!(k == i && l == j) && grid[k][l] != DEAD)
	    neighbors++;
	}
      }

      // update state
      if (neighbors < LOWER_THRESH || neighbors > UPPER_THRESH)
	next_grid[i][j] = DEAD;
      else if (grid[i][j] != DEAD || neighbors == SPAWN_THRESH)
	next_grid[i][j] = grid[i][j]+1;
    }
  }
}

/**
 * @brief Copies sides top and bottom
 *
 * Copies sides, top, and bottom to their respective 
 * locations.
 */
void copy_bounds (struct life_t * life) {
  int i,j;

  int ncols = life->ncols;
  int nrows = life->nrows;

  int ** grid = life->grid;

  for (j = 0; j < nrows+2; j++) {
    grid[ncols+1][j] = grid[1][j];
    grid[0][j] = grid[ncols][j];
  }

  // copy corners
  grid[0][0]             = grid[0][nrows];
  grid[0][nrows+1]       = grid[0][1];
  grid[ncols+1][0]       = grid[ncols+1][nrows];
  grid[ncols+1][nrows+1] = grid[ncols+1][1];

  // copy top and bottom
  for (i = 1; i <= ncols; i++) {
    grid[i][0]       = grid[i][nrows];
    grid[i][nrows+1] = grid[i][1];
  }
}


/**
 * @brief Update temporary grid to main grid
 *
 * Copies values that were calculated by
 * the evaluate_rules function from from 
 * next_grid into grid.
 */
void update_grid (struct life_t * life) {
  int i,j;
  int ncols = life->ncols;
  int nrows = life->nrows;
  int ** grid      = life->grid;
  int ** next_grid = life->next_grid;

  for (i = 0; i < ncols+2; i++)
    for (j = 0; j < nrows+2; j++){
      grid[i][j] = next_grid[i][j];
    }
}

/**
 * @brief Allocate memory
 *
 * Allocates memory for integer 2D arrays. Two
 * grids are used and allocated here: The main
 * game grid and the update grid.
 */
void allocate_grids (struct life_t * life) {
  int i;
  int ncols = life->ncols;
  int nrows = life->nrows;

  life->grid      = (int **) malloc(sizeof(int *) * (ncols+2));
  life->next_grid = (int **) malloc(sizeof(int *) * (ncols+2));

  for (i = 0; i < ncols+2; i++) {
    life->grid[i]      = (int *) malloc(sizeof(int) * (nrows+2));
    life->next_grid[i] = (int *) malloc(sizeof(int) * (nrows+2));
  }
}


/**
 * @brief initialize grid with values
 *
 * Initialize cells based on input file, otherwise all cells
 * are first set dead then the randomize_grid funciton is
 * called.
 */
void init_grids (struct life_t * life) {

  FILE * fd;
  int i,j;

  if (life->infile != NULL) {
    if ((fd = fopen(life->infile, "r")) == NULL) {
      perror("Failed to open file for input");
      exit(EXIT_FAILURE);
    }

    if (fscanf(fd, "%d %d\n", &life->ncols, &life->nrows) == EOF) {
      printf("File must at least define grid dimensions!\nExiting.\n");
      exit(EXIT_FAILURE);
    }
  }

  allocate_grids(life);

  for (i = 0; i < life->ncols+2; i++) {
    for (j = 0; j < life->nrows+2; j++) {
      life->grid[i][j]      = DEAD;
      life->next_grid[i][j] = DEAD;
    }
  }

  if (life->infile != NULL) {
    while (fscanf(fd, "%d %d\n", &i, &j) != EOF) {
      life->grid[i][j]      = ALIVE;
      life->next_grid[i][j] = ALIVE;
    }

    fclose(fd);
  } else {
    randomize_grid(life, INIT_PROB);
  }
  
}

/**
 * @brief Write current game state to file
 * 
 * Writes the current state of the main grid to the outfile
 * indicated in the commandline args. The output is basically
 * the coordinates of all cells alive separated with a space.
 */
void write_grid (struct life_t * life) {
  FILE * fd;
  int i,j;
  int ncols   = life->ncols;
  int nrows   = life->nrows;
  int ** grid = life->grid;

  if (life->outfile != NULL) {
    if ((fd = fopen(life->outfile, "w")) == NULL) {
      perror("Failed to open file for output");
      exit(EXIT_FAILURE);
    }

    fprintf(fd, "%d %d\n", ncols, nrows);

    for (i = 1; i <= ncols; i++) {
      for (j = 1; j <= nrows; j++) {
	if (grid[i][j] != DEAD)
	  fprintf(fd, "%d %d\n", i, j);
      }
    }
    fclose(fd);
  }
}


/**
 * @brief Frees the memory
 *
 * Frees memory of all three grids that were
 * allocated with in the call to allocate_grids().
 */
void free_grids (struct life_t * life) {
  int i;
  int ncols = life->ncols;

  for (i = 0; i < ncols+2; i++) {
    free(life->grid[i]);
    free(life->next_grid[i]);
  }

  free(life->grid);
  free(life->next_grid);
}

/**
 *  @brief Generate a random double between 0 and 1.
 */
double rand_double() {
  return (double)random()/(double)RAND_MAX;
}

/**
 * @brief initializes the main grid
 *
 * Using a fix probability to determine whether 
 * a cell is dead or alive on startup if no 
 * in file is provided
 */
void randomize_grid (struct life_t * life, double prob) {
  int i,j;
  int ncols = life->ncols;
  int nrows = life->nrows;

  for (i = 1; i <= ncols; i++) {
    for (j = 1; j <= nrows; j++) {
      if (rand_double() < prob)
	life->grid[i][j] = ALIVE;
    }
  }
}


/**
 * @brief seed random number generator
 *
 * Use rank and time to seed the random number
 * generator.
 */
void seed_random (int rank) {
  srandom(time(NULL) + 100*rank);
}

/**
 * @brief cleaning up after the game
 *
 * clean up handles output of the grid and
 * calling the free memory routines.
 */
void cleanup (struct life_t * life) {
  write_grid(life);
  free_grids(life);
}
