#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

long double sum=0;
pthread_barrier_t barr;

struct thread_attr {
    unsigned long thread_id;
    unsigned long thread_count;
    unsigned long n;
};

void* MonteCarlo(void* rank) {
    unsigned long tid = ((struct thread_attr*) rank) -> thread_id;
    unsigned long tcount = ((struct thread_attr*) rank) -> n / ((struct thread_attr*) rank) -> thread_count;
    unsigned long my_first_count = tcount * tid;
    unsigned long my_last_count = my_first_count + tcount;
    long double my_sum = 0;
    long double factor = my_first_count % 2 == 0 ? 1 : -1;
	unsigned long i;
	   
	//float x, y;
	//unsigned int seed = tid;
	//unsigned long count;
    for (i = my_first_count; i < my_last_count; i++, factor = -factor)
    {
        my_sum += factor / (2 * i + 1);
        // MonteCarlo
	    //x = (float) rand_r(&seed) / RAND_MAX;
	    //y = (float) rand_r(&seed) / RAND_MAX;
        //if ( x * x + y * y  < 1) count++;
        //printf("Thread=%ld,count=%d\n",tid,count);
    }
	
    /* wait barrier */
    pthread_barrier_wait(&barr);
	sum += my_sum;

    return NULL;
}

int main(int argc, char* argv[]) {
    clock_t t1, t2;
    t1 = clock();
    unsigned long thread;
    unsigned long thread_count = strtoul(argv[1], NULL, 10);
    unsigned long n = strtoul(argv[2], NULL, 10);
    struct thread_attr* thread_attribute = malloc(thread_count * sizeof(struct thread_attr));
    pthread_t* thread_handles = malloc(thread_count * sizeof(pthread_t));

    /* call argc */
    //printf("Input argument numbers: %d\n", argc);

    pthread_barrier_init(&barr, NULL, (unsigned) thread_count);

    /* thread starting*/
    for (thread = 0; thread < thread_count; thread++) {
        thread_attribute[thread].thread_id = thread;
        thread_attribute[thread].thread_count = thread_count;
        thread_attribute[thread].n = n;
        pthread_create(&thread_handles[thread], NULL, MonteCarlo, (void*) &thread_attribute[thread]);
    }

    /* thread join */
    for (thread = 0; thread < thread_count; thread++) 
		pthread_join(thread_handles[thread], NULL);

	
    printf("Input %lu threads and count = %lu times,\n",thread_count, n);
    printf("Pi calculation result= %.15Lf\n", 4 * sum);


    /* destroy barrier */
    pthread_barrier_destroy(&barr);
    free(thread_attribute);
    free(thread_handles);
    
    t2 = clock();
    printf("Spent time = %lf seconds.\n", (t2-t1)/(double)(CLOCKS_PER_SEC));
    
    return 0;
}
