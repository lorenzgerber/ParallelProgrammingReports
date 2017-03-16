#include "Life.h"

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
