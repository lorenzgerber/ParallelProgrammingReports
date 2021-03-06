/* File:     pth_thread_pool.c
 *
 * Purpose:  Implement a thread pool task queue with sorted linked list 
             operations.
 * 
 * Compile:  gcc -g -Wall -o pth_thread_pool pth_thread_pool.c 
 *              my_rand.c -lpthread
 *           needs timer.h and my_rand.h
 *
 * Usage:    ./pth_thread_cound <thread_count>
 * Input:    total number of keys inserted by main thread
 *           total number of work blocks to assign
 *           percent of ops that are searches and inserts (remaining ops
 *              are deletes.
 * Output:   Elapsed time to carry out the ops
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_rand.h"
#include "timer.h"

/* Random ints are less than MAX_KEY */
const int MAX_KEY = 100000000;

/* Struct for list nodes */
struct list_node_s {
   int    data;
   struct list_node_s* next;
};

/* Shared variables */
struct      list_node_s* head = NULL;  
int         thread_count;
int         total_ops;
double      insert_percent;
double      search_percent;
double      delete_percent;
pthread_mutex_t mutex;
pthread_mutex_t count_mutex;
pthread_mutex_t cond_mutex;
pthread_cond_t cond_var;
int         member_total=0, insert_total=0, delete_total=0;
double work_block[10] = {0};
int no_work_blocks;
int finish_job = 0;
int thread_assignment = 0;

/* Setup and cleanup */
void        Usage(char* prog_name);
void        Get_input(int* inserts_in_main_p);

/* Thread function */
void*       Thread_work(void* rank);

/* List operations */
int         Insert(int value);
void        Print(void);
int         Member(int value);
int         Delete(int value);
void        Free_list(void);
int         Is_empty(void);

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long i; 
   int key, success, attempts;
   pthread_t* thread_handles;
   int inserts_in_main;
   unsigned seed = 1;
   double start, finish;

   int work_counter;
   int worker_id = 0;

   /* take care of command line args */
   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1],NULL,10);

   /* Get user input */
   Get_input(&inserts_in_main);

   /* Try to insert inserts_in_main keys, but give up after */
   /* 2*inserts_in_main attempts.                           */
   i = attempts = 0;
   while ( i < inserts_in_main && attempts < 2*inserts_in_main ) {
      key = my_rand(&seed) % MAX_KEY;
      success = Insert(key);
      attempts++;
      if (success) i++;
   }
   printf("Inserted %ld keys in empty list\n", i);


   /* Prepare threads and mutexes */
   thread_handles = malloc((thread_count-1)*sizeof(pthread_t));
   pthread_mutex_init(&mutex, NULL);
   pthread_mutex_init(&count_mutex, NULL);

   GET_TIME(start);
   /* start threads */
   for (i = 1; i <= thread_count; i++)
      pthread_create(&thread_handles[i], NULL, Thread_work, (void*) i);

   /* Generate work blocks */
   for (work_counter = 0; work_counter < no_work_blocks; work_counter++){
    
     /* check conditional, if zero, proceed */
     pthread_mutex_lock(&cond_mutex);
     while(thread_assignment != 0){
       pthread_cond_wait(&cond_var, &cond_mutex);
     }

     /* fill work block */
     for(i = 0; i < 10; i++){
       work_block[i] = my_drand(&seed);
     }

     /* assign thread for work */
     worker_id = (my_rand(&seed) % (thread_count-1)) + 1;
     thread_assignment = worker_id;     
     pthread_mutex_unlock(&cond_mutex);

   }
   
   /* wait until last thread is in work loop */
   pthread_mutex_lock(&cond_mutex);
   while(thread_assignment != 0){
     pthread_cond_wait(&cond_var, &cond_mutex);
   }  

   /* set finish_job to true */
   finish_job = 1;

   /* broadcast threads */
   pthread_cond_broadcast(&cond_var);
   pthread_mutex_unlock(&cond_mutex);
   
   /* join threads after done work */
   for (i = 1; i <= thread_count; i++){
     pthread_join(thread_handles[i], NULL);
   }

   /* report results */
   GET_TIME(finish);
   printf("Elapsed time = %e seconds\n", finish - start);
   printf("Total ops = %d\n", no_work_blocks * 10);
   printf("member ops = %d\n", member_total);
   printf("insert ops = %d\n", insert_total);
   printf("delete ops = %d\n", delete_total);

   /* clean up */
   Free_list();
   pthread_mutex_destroy(&mutex);
   pthread_mutex_destroy(&count_mutex);
   pthread_mutex_destroy(&cond_mutex);
   pthread_cond_destroy(&cond_var);
   free(thread_handles);

   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
}  /* Usage */

/*-----------------------------------------------------------------*/
void Get_input(int* inserts_in_main_p) {

   printf("How many keys should be inserted in the main thread?\n");
   scanf("%d", inserts_in_main_p);
   printf("How many work blocks of each 10 operations should be executed?\n");
   scanf("%d", &no_work_blocks);
   printf("Percent of ops that should be searches? (between 0 and 1)\n");
   scanf("%lf", &search_percent);
   printf("Percent of ops that should be inserts? (between 0 and 1)\n");
   scanf("%lf", &insert_percent);
   delete_percent = 1.0 - (search_percent + insert_percent);
}  /* Get_input */

/*-----------------------------------------------------------------*/
/* Insert value in correct numerical location into list */
/* If value is not in list, return 1, else return 0 */
int Insert(int value) {
   struct list_node_s* curr = head;
   struct list_node_s* pred = NULL;
   struct list_node_s* temp;
   int rv = 1;
   
   while (curr != NULL && curr->data < value) {
      pred = curr;
      curr = curr->next;
   }

   if (curr == NULL || curr->data > value) {
      temp = malloc(sizeof(struct list_node_s));
      temp->data = value;
      temp->next = curr;
      if (pred == NULL)
         head = temp;
      else
         pred->next = temp;
   } else { /* value in list */
      rv = 0;
   }

   return rv;
}  /* Insert */

/*-----------------------------------------------------------------*/
void Print(void) {
   struct list_node_s* temp;

   printf("list = ");

   temp = head;
   while (temp != (struct list_node_s*) NULL) {
      printf("%d ", temp->data);
      temp = temp->next;
   }
   printf("\n");
}  /* Print */


/*-----------------------------------------------------------------*/
int  Member(int value) {
   struct list_node_s* temp;

   temp = head;
   while (temp != NULL && temp->data < value)
      temp = temp->next;

   if (temp == NULL || temp->data > value) {
#     ifdef DEBUG
      printf("%d is not in the list\n", value);
#     endif
      return 0;
   } else {
#     ifdef DEBUG
      printf("%d is in the list\n", value);
#     endif
      return 1;
   }
}  /* Member */

/*-----------------------------------------------------------------*/
/* Deletes value from list */
/* If value is in list, return 1, else return 0 */
int Delete(int value) {
   struct list_node_s* curr = head;
   struct list_node_s* pred = NULL;
   int rv = 1;

   /* Find value */
   while (curr != NULL && curr->data < value) {
      pred = curr;
      curr = curr->next;
   }
   
   if (curr != NULL && curr->data == value) {
      if (pred == NULL) { /* first element in list */
         head = curr->next;
#        ifdef DEBUG
         printf("Freeing %d\n", value);
#        endif
         free(curr);
      } else { 
         pred->next = curr->next;
#        ifdef DEBUG
         printf("Freeing %d\n", value);
#        endif
         free(curr);
      }
   } else { /* Not in list */
      rv = 0;
   }

   return rv;
}  /* Delete */

/*-----------------------------------------------------------------*/
void Free_list(void) {
   struct list_node_s* current;
   struct list_node_s* following;

   if (Is_empty()) return;
   current = head; 
   following = current->next;
   while (following != NULL) {
#     ifdef DEBUG
      printf("Freeing %d\n", current->data);
#     endif
      free(current);
      current = following;
      following = current->next;
   }
#  ifdef DEBUG
   printf("Freeing %d\n", current->data);
#  endif
   free(current);
}  /* Free_list */

/*-----------------------------------------------------------------*/
int  Is_empty(void) {
   if (head == NULL)
      return 1;
   else
      return 0;
}  /* Is_empty */

/*-----------------------------------------------------------------*/
void* Thread_work(void* rank) {
   long my_rank = (long) rank;
   int i, val;
   unsigned seed = my_rank + 1;
   int my_member=0, my_insert=0, my_delete=0;
   double local_work_block[10] = {0};

   while(1){

     /* go to conditional sleep */
     pthread_mutex_lock(&cond_mutex);

     while(thread_assignment != my_rank && finish_job == 0){
       pthread_cond_wait(&cond_var, &cond_mutex);
     }

     /* check project finished */
     if (finish_job){
       return NULL;
     }

     /* if my turn read, global work block to local work block */
     for (i = 0; i < 10; i++){
       local_work_block[i] = work_block[i];
     }

     /* set thread_assignment back to zero */
     thread_assignment = 0;

     /* do work */
     for (i = 0; i < 10; i++) {
       val = my_rand(&seed) % MAX_KEY;
       if (local_work_block[i] < search_percent) {
           pthread_mutex_lock(&mutex);
           Member(val);
           pthread_mutex_unlock(&mutex);
           my_member++;
        } else if (local_work_block[i] < search_percent + insert_percent) {
           pthread_mutex_lock(&mutex);
           Insert(val);
           pthread_mutex_unlock(&mutex);
           my_insert++;
        } else { /* delete */
           pthread_mutex_lock(&mutex);
           Delete(val);
           pthread_mutex_unlock(&mutex);
           my_delete++;
        }

     }  /* for */

     /* book keeping for results */
     pthread_mutex_lock(&count_mutex);
     member_total += my_member;
     insert_total += my_insert;
     delete_total += my_delete;

     pthread_mutex_unlock(&count_mutex);
     my_member = my_insert = my_delete = 0;

   } /* while */


   return NULL;
}  /* Thread_work */
