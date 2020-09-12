 #include <pthread.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <time.h>
 #include <omp.h>

 //#define NUM_THREADS	4
 //#define number_of_tosses 10000000
int NUM_THREADS=0;
long number_of_tosses=0;

long total_number_in_circle=0;
float pi_estimate;
long tid_num_of_circle[3];

//pthread_barrier_t our_barrier;

float func(float a) /* 四舍五入到小数点后2位 */
{
    return (int)(a*100+0.5)/100.0;
}

 void *MonteCarlo(void *t)
 {
    unsigned int toss, my_random;
    unsigned long tid;
    float x, y;
    unsigned long number_in_circle = 0;
    //double pi_estimate;

    tid = (long) t;
    unsigned int seed = tid;
    //printf("Thread %ld starting...\n",tid);
    for ( toss = 0; toss < number_of_tosses/NUM_THREADS; toss ++)
	{
	    //x =( rand()%99 / 100.0);
	    //y =( rand()%99 / 100.0);

	    x = (float) rand_r(&seed) / RAND_MAX;
	    y = (float) rand_r(&seed) / RAND_MAX;
        //x = (float) rand() / RAND_MAX;
        //y = (float) rand() / RAND_MAX;
        //distance_squared = x * x + y * y ;
        if ( x * x + y * y  <= 1) number_in_circle ++;
        //printf("x=%f, y=%f, distance=%f \n", x, y,distance_squared);
    }
    tid_num_of_circle[tid] = number_in_circle;
    //printf("tid=%ld, number=%ld, array=%ld\n", tid, number_in_circle, tid_num_of_circle[tid]);
    //printf("%ld \n", number_in_circle);
    //pi_estimate = 4* number_in_circle /(( double ) number_of_tosses ) ;
    //printf("Thread %ld done. Result = %e\n",tid, pi_estimate);

    pthread_exit((void*) t);
 }

 int main (int argc, char *argv[])
 {

    clock_t t1, t2;
    t1 = clock();

    NUM_THREADS = strtoul(argv[1], NULL, 10);
    number_of_tosses = strtoul(argv[2], NULL, 10);
    //printf("Thread=%d \n", NUM_THREADS);
    //printf("tosses=%ld \n", number_of_tosses);

    pthread_t thread[NUM_THREADS];
    //pthread_attr_t attr;
    int rc;
    long t;
    void *status;

    /* Initialize and set thread detached attribute */
    //pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //pthread_barrier_init(&our_barrier,NULL,4);
    for(t=0; t<NUM_THREADS; t++)
	{
       //printf("Main: creating thread %ld\n", t);
       pthread_create(&thread[t], NULL, MonteCarlo, (void*) t);
    }

    /* Free attribute and wait for the other threads */
    //pthread_attr_destroy(&attr);
    for(t=0; t<NUM_THREADS; t++)
	{
       	pthread_join(thread[t], &status);
    	//printf("Main: completed join with thread %ld having a status of %ld\n", t,);
    }

    for(t=0;t<NUM_THREADS;t++) total_number_in_circle+=tid_num_of_circle[t];

    //printf("Total num in circle=%ld \n", total_number_in_circle);
    pi_estimate = 4* total_number_in_circle /(( double ) number_of_tosses ) ;
    printf("Thread done. Result = %e\n", pi_estimate);
    printf("Main: program completed.\n");

    t2 = clock();
    printf("\tSpent Time = %lf\n", (t2-t1)/(double)(CLOCKS_PER_SEC));
    pthread_exit(NULL);
 }
