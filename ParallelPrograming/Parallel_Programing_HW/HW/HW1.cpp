 #include <pthread.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <time.h>

 #define NUM_THREADS	4
 #define number_of_tosses 10000000

 void *MonteCarlo(void *t)
 {
    int toss;
    long tid;
    //double result=0.0;
    tid = (long) t;
    printf("Thread %ld starting...\n",tid);

    double x, y, distance_squared, pi_estimate;
    long number_in_circle = 0;
    for ( toss = 0; toss < number_of_tosses ; toss ++) {
        x = (double)rand()*2 / RAND_MAX + (-1) ;
        y = (double)rand()*2 / RAND_MAX + (-1) ;
        distance_squared = x * x + y * y ;
        if ( distance_squared <= 1) number_in_circle ++;
    }
    pi_estimate = 4* number_in_circle /(( double ) number_of_tosses ) ;
    printf("Thread %ld done. Result = %e\n",tid, pi_estimate);
    //pthread_exit((void*) t);
 }

 int main (int argc, char *argv[])
 {
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    long t;
    void *status;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(t=0; t<NUM_THREADS; t++) {
       printf("Main: creating thread %ld\n", t);
       pthread_create(&thread[t], &attr, MonteCarlo, (void *)t);

       }

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for(t=0; t<NUM_THREADS; t++) {
       pthread_join(thread[t], &status);
       printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
       }

    printf("Main: program completed. Exiting.\n");

    //pthread_create(&thread[t], &attr, BusyWork2, (void *)5);

    pthread_exit(NULL);
 }
