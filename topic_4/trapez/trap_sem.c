/* File:    trap.c
 * Purpose: Calculate definite integral using trapezoidal 
 *          rule.
 *
 * Input:   a, b, n, thread_count
 * Output:  Estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Compile: gcc -g -Wall -o trap trap.c
 * Usage:   ./trap
 *
 * Note:    The function f(x) is hardwired. thread_count should 
 *          the current version be integer divisible by n-1 
 *
 * IPP:     Section 3.2.1 (pp. 94 and ff.) and 5.2 (p. 216)
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* global variables */
int thread_count;
double  integral;   /* Store result in integral   */
double  a, b;       /* Left and right endpoints   */
int     n;          /* Number of trapezoids       */
double  h;          /* Height of trapezoids       */
int  flag = 0; 
sem_t bin_sem;

/* serial functions */
double f(double x);    /* Function we're integrating */
void Trap(double a, double b, int n, double h);

/* parallel function */
void *Pth_interpol(void* rank);


int main(void) {

   printf("Enter a, b, n and thread_count\n");
   scanf("%lf", &a);
   scanf("%lf", &b);
   scanf("%d", &n);
   scanf("%d", &thread_count);

   sem_init(&bin_sem, 0, 1);

   h = (b-a)/n;
   Trap(a, b, n, h);
   
   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f = %.15f\n",
      a, b, integral);

   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Estimate integral from a to b of f using trap rule and
 *              n trapezoids
 * Input args:  a, b, n, h
 * Return val:  Estimate of the integral 
 */
void Trap(double a, double b, int n, double h) {
   long thread;
   pthread_t* thread_handles;

   integral = (f(a) + f(b))/2.0;


   thread_handles = malloc(thread_count*sizeof(pthread_t));

   for (thread = 0; thread < thread_count; thread++)
     pthread_create(&thread_handles[thread], NULL,
		    Pth_interpol, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
     pthread_join(thread_handles[thread], NULL);
  
   integral = integral*h;
   sem_destroy(&bin_sem);

}  /* Trap */

void *Pth_interpol(void* rank){
  /* not the first and not the last */
  long my_rank = (long) rank;
  int first_k = (n-1) / thread_count * my_rank + 1 ; 
  int last_k= (n-1) / thread_count * (my_rank+1);
  double local_integral = 0;
  int k;

  //printf("first %d last %d\n", first_k, last_k);

  for (k = first_k; k <= last_k; k++) {
     local_integral += f(a+k*h);
  }

  /* updating global sum */
  sem_wait(&bin_sem);
  integral += local_integral;
  sem_post(&bin_sem);

  return NULL;
  
}

/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input args:  x
 */
double f(double x) {
   double return_val;

   return_val = x*x;
   return return_val;
}  /* f */
