#include <stddef.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "timer.h"

struct life_t {
  int  rank;
  int  size;
  int  ncols;
  int  nrows;
  int  ** grid;
  int  ** next_grid;
  int  * transfer_grid;
  bool do_display;
  int  generations;
  char * infile;
  char * outfile;
};

enum CELL_STATES {
	DEAD = 0,
	ALIVE
};

const int     DEFAULT_SIZE = 105;
const int     DEFAULT_GENS = 10000;
const double     INIT_PROB = 0.25;
const int     UPPER_THRESH = 3;
const int     LOWER_THRESH = 2;
const int     SPAWN_THRESH = 3;

int                  init (struct life_t * life, int * c, char *** v);
void           eval_rules (struct life_t * life);
void          copy_bounds (struct life_t * life);
void          update_grid (struct life_t * life);
void       allocate_grids (struct life_t * life);
void           init_grids (struct life_t * life);
void           write_grid (struct life_t * life);
void           free_grids (struct life_t * life);
double        rand_double ();
void       randomize_grid (struct life_t * life, double prob);
void          seed_random (int rank);
void              cleanup (struct life_t * life);
void         collect_data (struct life_t * life);
void      distribute_data (struct life_t * life);

/**
 * init_env()
 * Initializes runtime environment.
 *
 */
int init (struct life_t * life, int * c, char *** v) {
  int argc          = *c;
  char ** argv      = *v;
  life->rank        = 0;
  life->size        = 1;
  life->ncols       = DEFAULT_SIZE;
  life->nrows       = DEFAULT_SIZE;
  life->generations = DEFAULT_GENS;
  life->outfile     = NULL;
  life->infile      = NULL;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &life->rank);
  MPI_Comm_size(MPI_COMM_WORLD, &life->size);

  if (argc >= 4){
    if(strtol(argv[1], (char**) NULL, 10)%life->size !=0){
      printf("the size n of the square has to be even divisible by the rank of nodes");
      exit(1);
    }
    life->ncols = strtol(argv[1], (char**) NULL, 10)/life->size;
    life->nrows = strtol(argv[1], (char**) NULL, 10);
    life->generations = strtol(argv[2], (char**) NULL, 10);
    life->outfile   = argv[3];
    if(argc == 5){
      life->infile = argv[4];
    }
    
  } else {
    printf("usage: mpi_life n_size generations outfile [infile]");
    exit(1);
  }

  seed_random(life->rank);
  init_grids(life);

  return 0;
}

/**
 *  eval_rules()
 *  Evaluate the rules of Life for each cell; count
 *  neighbors and update current state accordingly.
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
      //printf("%d %d %d %d %d\n", i , j, neighbors, grid[i][j], next_grid[i][j]);

      // update state
      if (neighbors < LOWER_THRESH || neighbors > UPPER_THRESH)
	next_grid[i][j] = DEAD;
      else if (grid[i][j] != DEAD || neighbors == SPAWN_THRESH)
	next_grid[i][j] = grid[i][j]+1;		
    }
  }
}

/**
 *  copy_bounds()
 *  Copies sides, top, and bottom to their respective locations.
 *  All boundaries are considered periodic.
 *
 *  In the MPI model, processes are aligned side-by-side.
 *  Left and right sides are sent to neighboring processes.
 *  Top and bottom are copied from the process's own grid.
 */
void copy_bounds (struct life_t * life) {
  int i,j;

  int rank  = life->rank;
  int size  = life->size;
  int ncols = life->ncols;
  int nrows = life->nrows;

  int ** grid = life->grid;

  MPI_Status status;
  int left_rank  = (rank-1+size) % size;
  int right_rank = (rank+1) % size;

  enum TAGS {
    TOLEFT,
    TORIGHT	
  };

  if (size != 1) {
    MPI_Sendrecv(grid[1], nrows+2, MPI_INT, left_rank, TOLEFT,
		 grid[ncols+1], nrows+2, MPI_INT, right_rank, TOLEFT,			
		 MPI_COMM_WORLD, &status);

		
    MPI_Sendrecv(grid[ncols], nrows+2, MPI_INT, right_rank,
		 TORIGHT, grid[0], nrows+2, MPI_INT, left_rank,
		 TORIGHT, MPI_COMM_WORLD, &status);
  }
	
  // Copy sides locally to maintain periodic boundaries
  // when there's only one process
  if (size == 1) {
    for (j = 0; j < nrows+2; j++) {
      grid[ncols+1][j] = grid[1][j];		
      grid[0][j] = grid[ncols][j];	
    }	
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
 *  update_grid()
 *  Copies temporary values from next_grid into grid.
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
 *  allocate_grids()
 *  Allocates memory for a 2D array of integers.
 */
void allocate_grids (struct life_t * life) {
  int i;
  int ncols = life->ncols;
  int nrows = life->nrows;
  int * data_grid;
  int * data_next;

  // allocating the transfer array
  if(life->rank == 0){
    life->transfer_grid = (int *) malloc(sizeof(int ) * (ncols + 2) * life->size * (nrows + 2));
  }

  // main grids
  life->grid      = (int **) malloc(sizeof(int *) * (ncols+2));
  life->next_grid = (int **) malloc(sizeof(int *) * (ncols+2));
  data_grid = malloc((nrows+2)*(ncols+2)*sizeof(int));
  data_next = malloc((nrows+2)*(ncols+2)*sizeof(int));
  for (i = 0; i < ncols+2; i++){
    life->grid[i] = &(data_grid[i*(nrows+2)]);
    life->next_grid[i] = &(data_next[i*(nrows+2)]);		      
  }

}

/**
 *  init_grids()
 *  Initialize cells based on input file, otherwise all cells
 *  are DEAD.
 */
void init_grids (struct life_t * life) {
  int i,j;
  int index;
  FILE * fd;

  if (life->infile != NULL && life->rank == 0) {
    if ((fd = fopen(life->infile, "r")) == NULL) {
      perror("Failed to open file for input");
      exit(EXIT_FAILURE);
    }

    if (fscanf(fd, "%d %d\n", &life->ncols, &life->nrows) == EOF) {
      printf("File must at least define grid dimensions!\nExiting.\n");
      exit(EXIT_FAILURE);
    }
    life->ncols = life->ncols / life->size;    
  }

  if(life->infile != NULL){
    MPI_Bcast(&life->ncols, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }
  
  allocate_grids(life);

  for (i = 0; i < life->ncols+2; i++) {
    for (j = 0; j < life->nrows+2; j++) {
      life->grid[i][j]      = DEAD;
      life->next_grid[i][j] = DEAD;
    }
  }

  
  if (life->infile != NULL && life->rank == 0) {
    while (fscanf(fd, "%d %d\n", &i, &j) != EOF) {
      index = ((i-1)/(life->ncols))*(life->ncols+2)*(life->nrows+2)+(life->nrows+2)+(((i-1)%(life->ncols))*(life->nrows+2))+(j);
      life->transfer_grid[index] = ALIVE;
    }

    fclose(fd);

  } else {
    randomize_grid(life, INIT_PROB);
  }

  if (life->infile != NULL){
    distribute_data(life);
  }

}

/**
 *  write_grid()
 *  Dumps the current state of life.grid to life.outfile.
 *  Only outputs the coordinates of !DEAD cells.
 */
void write_grid (struct life_t * life) {
  FILE * fd;
  int i,j,k;
  int col, row, index;
  int ncols   = life->ncols;
  int nrows   = life->nrows;


  if (life->outfile != NULL) {       
    if ((fd = fopen(life->outfile, "w")) == NULL) {		
      perror("Failed to open file for output");
      exit(EXIT_FAILURE);		
    }
	
    fprintf(fd, "%d %d\n", ncols*life->size, nrows);


    for(i = 0; i < life->size; i++){
      for( j = 0; j < ncols + 1; j++){
	for( k = 0; k < nrows + 1; k++){

	  index = i*(nrows+2)*(ncols+2)+j*(nrows+2)+k;
	  col = i*ncols+j;
	  row = k;
	    
	  if (life->transfer_grid[index] != DEAD){
	    fprintf(fd, "%d %d\n", col, row);
	  }	  
	}
      }
    }
    
    fclose(fd);
  }
}

/**
 *  free_grids()
 *  Frees memory used by an array that was allocated 
 *  with allocate_grids().
 */
void free_grids (struct life_t * life) {
	
  free(life->grid[0]);
  free(life->next_grid[0]);
  
  free(life->grid);
  free(life->next_grid);

}

/**
 *  rand_double()
 *  Generate a random double between 0 and 1.
 */
double rand_double() {
  return (double)rand()/(double)RAND_MAX;
}

/**
 *  randomize_grid()
 *  Initialize a Life grid. Each cell has a [prob] chance
 *  of starting alive.
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
 *  seed_random()
 *  Seed the random number generator based on the
 *  process's rank and time. Multiplier is arbitrary.
 */
void seed_random (int rank) {
  srand(time(NULL) + 100*rank);
}

/**
 *  cleanup()
 *  Prepare process for a clean termination.
 */
void cleanup (struct life_t * life) {

  collect_data(life);
  
  if (life->rank == 0){
    write_grid(life);
  }

  free_grids(life);
  MPI_Finalize();
}

/**
 * collect_data()
 * collects the data from all nodes onto node 0
 *
 */
void collect_data(struct life_t * life){
  MPI_Gather(life->grid[0], (life->nrows+2)*(life->ncols+2), MPI_INT, life->transfer_grid, (life->nrows+2)*(life->ncols+2), MPI_INT, 0, MPI_COMM_WORLD);
  
}

/**
 * distribute_data()
 * distributes the read file to the different processes
 *
 */
void distribute_data(struct life_t *life){
  MPI_Scatter(life->transfer_grid, (life->nrows+2)*(life->ncols+2), MPI_INT, life->grid[0], (life->nrows+2)*(life->ncols+2), MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(life->transfer_grid, (life->nrows+2)*(life->ncols+2), MPI_INT, life->next_grid[0], (life->nrows+2)*(life->ncols+2), MPI_INT, 0, MPI_COMM_WORLD);
}
