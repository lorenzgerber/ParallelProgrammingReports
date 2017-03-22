#include "Life.h"

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
  printf("%e\n", finish-start);

  cleanup(&life);
  exit(EXIT_SUCCESS);

}
