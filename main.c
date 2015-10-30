#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


// struct to pass as param to thread
typedef struct thread_data_t {
    int value;
    char c;
} thread_data_t;


void thread1(void *arg);
void thread2(void *arg);


// semaphore that thread2's wait on
sem_t allowThread2;


int main(int argc, char const *argv[]) {
   // initialize semaphore to 0
   sem_init(&allowThread2, 0, (unsigned int)0);

   // declare threads
   pthread_t t1;
   pthread_t t2;
   pthread_t t3;
   pthread_t t4;

   // create thread params and set values
   thread_data_t *shared = malloc(sizeof(*shared));
   shared->value = 10;
   shared->c = 'b';

   thread_data_t *shared2 = malloc(sizeof(*shared));
   shared2->value = 5;
   shared2->c = 'a';

   // create and start threads with specified params and function to execute
   pthread_create(&t1, NULL, thread1, shared);
   pthread_create(&t2, NULL, thread1, shared2);
   pthread_create(&t3, NULL, thread2, shared);
   pthread_create(&t4, NULL, thread2, shared2);

   // wait for all threads to finish
   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
   pthread_join(t3, NULL);
   pthread_join(t4, NULL);

   return 0;
}

void thread1(void * arg) {

   // cast param to thread_data_t*
   thread_data_t *d = (thread_data_t *)arg;

   // get char and int from param struct
   int a = d->value;
   char c = d->c;
   
   // print char and int
   printf("%c%d\n",c, a);

   // signal semaphore
   sem_post(&allowThread2);
}

void thread2(void *arg) {
   
   // wait until thread2's are allowed
   sem_wait(&allowThread2);

   // get char from param
   char c = ((thread_data_t *)arg)->c;

   // print char
   printf("%c\n", c);
}
