#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define START_N 10
#define START_P 1
#define N_LENGTH 6
#define P_LENGTH 8

int main (void) {
  
  FILE *f = NULL;
  if((f = fopen("speedup.txt", "w"))<0){
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  FILE *g = NULL;

  if((g = fopen("efficiency.txt", "w"))<0){
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  double n = START_N;
  double p = START_P;

  double speedUp = 0;
  double efficiency = 0;

  for(int i = 0 ; i < N_LENGTH; i++){
    for(int j = 0; j < P_LENGTH; j++){

      speedUp = n*n / ((n * n) /  p  + (log(p)/log(2)));
      efficiency = n*n / (p * (n * n) / p + (log(p)/log(2))); 
      fprintf(f, "%lf\t", speedUp);
      fprintf(g, "%lf\t", efficiency);
      printf("n = %lf, p = %lf, speedup = %lf, efficiency = %lf\n", n, p, speedUp, efficiency);
      p*=2;
    }

    fprintf(f, "\n");
    fprintf(g, "\n");
    n*=2;
    p = START_P;
  }

  fclose(f);
  fclose(g);
  return 0;
}

