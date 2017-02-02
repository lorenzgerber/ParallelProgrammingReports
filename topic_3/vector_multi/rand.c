#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main (void){
  srand(time(NULL));
  for(int i = 0; i < 10;i++){

    int r = rand() % 1000;
    printf("%d\n",r);

  }
  
  return 0;
}
