/* File:    trap.c
 * Purpose: Calculate definite integral using trapezoidal 
 *          rule.
 *
 * Input:   a, b, n
 * Output:  Estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Compile: gcc -g -Wall -o trap trap.c
 * Usage:   ./trap
 *
 * Note:    The function f(x) is hardwired.
 *
 * IPP:     Section 3.2.1 (pp. 94 and ff.) and 5.2 (p. 216)
 */

#include <stdio.h>

/* global variables */
int thread_count;
double  integral;   /* Store result in integral   */
double  a, b;       /* Left and right endpoints   */
int     n;          /* Number of trapezoids       */
double  h;          /* Height of trapezoids       */

/* serial functions */
double f(double x);    /* Function we're integrating */
double Trap(double a, double b, int n, double h);

/* parallel function */
void *Pth_interpol(void* rank);


int main(void) {

  
   

   printf("Enter a, b, n and thread_count\n");
   scanf("%lf", &a);
   scanf("%lf", &b);
   scanf("%d", &n);
   scanf("%d", &thread_count);

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
   double integral;
   int k;
   long thread;
   pthread_t* thread_handles;

   integral = (f(a) + f(b))/2.0;

   /* here comes the parallel part */

   thread_handles = malloc(thread_count*sizeof(pthread_t));

   for (thread = 0; thread < thread_count; thread++)
     pthread_create(&thread_handles[thread], NULL,
		    Pth_mat_vect, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
     pthread_join(thread_handles[thread], NULL);
   
   
   integral = integral*h;

   return NULL
}  /* Trap */

void *Pth_interpol(void* rank){
  /* not the first and not the last */
  int first_k; // was 1
  int last_k;  // was n-1

  for (k = first_k; k <= last_k; k++) {
     integral += f(a+k*h);
  }

  


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
