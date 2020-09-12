#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

long number_of_tosses = 0;
long double sum = 0;
pthread_barrier_t barr;

struct thread_data {
    unsigned long thread_id;
    unsigned long thread_count;
    unsigned long n;
};
 
void* Threading(void* rank) {
    unsigned long tid = ((struct thread_data*) rank) -> thread_id;
    unsigned long tcount = ((struct thread_data*) rank) -> n / ((struct thread_data*) rank) -> thread_count;
    unsigned long my_first_count = tcount * tid;
    unsigned long my_last_count = my_first_count + tcount;
    long double my_sum = 0;
    long double factor = my_first_count % 2 == 0 ? 1 : -1;
    unsigned long i;
 
    for (i = my_first_count; i < my_last_count; i++, factor = -factor)
        my_sum += factor / (2 * i + 1);
 
    /* barrier︰等 threads 全都到齊才一起執行 */
    pthread_barrier_wait(&barr);
    sum += my_sum;
 
    return NULL;
}

 void *MonteCarlo(void *t)
 {
    int toss;
    long tid;
    long number_in_circle = 0;
    double x, y, pi_estimate, distance_squared;
    
    tid =(long) t;	    
    printf("Thread %ld starting...\n",tid);
    for ( toss = 0; toss < number_of_tosses ; toss ++) 
	{
        x = (double)rand()*2 / RAND_MAX + (-1) ;
        y = (double)rand()*2 / RAND_MAX + (-1) ;
        distance_squared = x * x + y * y ;
        if ( distance_squared <= 1) number_in_circle ++;
        
        pthread_barrier_wait(&barr);
        sum += number_in_circle;
    }
    pi_estimate = 4* number_in_circle /(( double ) number_of_tosses ) ;
    printf("Thread %ld done. Result = %e\n",tid, pi_estimate);
 }
 
int main(int argc, char* argv[]) {
    unsigned long thread;
    unsigned long thread_count = strtoul(argv[1], NULL, 10);
    unsigned long n = strtoul(argv[2], NULL, 10);
    struct thread_data *thread_attribute = malloc(thread_count * sizeof(struct thread_data));
    pthread_t *thread_handles = malloc(thread_count * sizeof(pthread_t));
 	
 	number_of_tosses = n;
    /* 呼叫 argc，避免 -Wextra 編譯警告 */
    printf("Input argument numbers: %d\n", argc);
 	printf("With n = %lu terms and %lu threads,\n", n, thread_count);
 	
    pthread_barrier_init(&barr, NULL, (unsigned) thread_count);
 
    /* 分流 thread */
    for (thread = 0; thread < thread_count; thread++) {
        thread_attribute[thread].thread_id = thread;
        thread_attribute[thread].thread_count = thread_count;
        thread_attribute[thread].n = n;
        pthread_create(&thread_handles[thread], NULL, MonteCarlo, (void*) &thread_attribute[thread]);
    }
 
    /* 合流 thread */
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL); 
 
    //printf("With n = %lu terms and %lu threads,\n", n, thread_count);
    //printf("\tOur estimate of pi = %.15Lf\n", 4 * sum);
    //printf("\tpi = %.15f\n", 4.0 * atan(1.0));
 
    /* 終止 barrier */
    pthread_barrier_destroy(&barr);
    free(thread_attribute);
    free(thread_handles);
    return 0;
}
