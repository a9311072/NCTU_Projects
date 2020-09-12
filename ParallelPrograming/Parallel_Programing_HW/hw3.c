#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "mpi.h"



int main(int argc, char **argv) {
    
    int N, seed, line;

    N = atoi(argv[1]);
    seed = atoi(argv[2]);
    srand(seed);

    int temp[N][N], next[N][N];
        for (int i = 0 ; i < N; i++){
            for (int j = 0; j < N; j++) {
                temp[i][j] = random() >> 3; 
                //MPI_Send(&temp[i][j], 1, MPI_INT, 0, MPI_COMM_WORLD);
            }
        }

    clock_t t1, t2, t3, t4;
    t1 = clock();

    int count = 0, balance = 0;
    

    int ranks, numprocs;
    MPI_Request req;
    MPI_Status status;

    MPI_Init( &argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ranks);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    
    line = N/numprocs;//将数据分为(进程数)个块,主进程也要处理数据


    if(ranks==0){


        // t3 = clock();
        // printf("Spent Time = %lf, Rank= %d\n", (t3-t1)/(double)(CLOCKS_PER_SEC), ranks);

        // Check the temp array data
        // for (int i = 0 ; i < N; i++)
        //     for (int j = 0; j < N; j++) 
        //         printf("temp[%d][%d]= %d \n",i,j, temp[i][j]);

        for (int i=1;i<numprocs;i++)
            MPI_Send(temp,N*N,MPI_INT,i,0,MPI_COMM_WORLD);
        //MPI_Bcast(temp, N*N, MPI_INT, 0, MPI_COMM_WORLD);

        
        for (int i=1;i<numprocs;i++) 
            MPI_Recv(temp, N*N, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // for (int k=1;k<numprocs;k++)
        //   for (int i=(k-1) * line; i<((k-1) * line + line); i++)
        //     for (int j=0;j<N;j++)
        //       MPI_Recv(&temp[i][j],1,  MPI_INT, k,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
       
        
        MPI_Recv(&count, 1, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // for (int k=1;k<numprocs;k++) {
        //     for (int i = 0 ; i < N; i++) {
        //         for (int j = 0; j < N; j++) {
        //             MPI_Recv(&temp[i][j],1,  MPI_INT, k,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        //             printf("temp[%d][%d]= %d \n",i,j, temp[i][j]);
        //         }
        //     }
        // }

        // for (int i = 0 ; i < N; i++)
        //     for (int j = 0; j < N; j++) 
        //         printf("next[%d][%d]= %d \n",i,j, next[i][j]);


        // printf("Size: %d*%d, Seed: %d, ", N, N, seed);
        // printf("Iteration: %d, Temp: %d\n, Ranks=%d \n", count, temp[0][0], ranks);
    }
    else {
        //printf("num of process= %d / %d \n", ranks,numprocs);

        MPI_Recv(temp, N*N ,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        //MPI_Allreduce(&next, &temp, N*N, MPI_INT, MPI_MAX,MPI_COMM_WORLD);

        // for (int i = 0 ; i < N; i++)
        //     for (int j = 0; j < N; j++) 
        //         printf("temp[%d][%d]= %d \n",i,j, temp[i][j]);

        count = 0, balance = 0;
        while (!balance) {
            balance = 1;
            count++;
            //MPI_Send(count,1,MPI_INT,0,2,MPI_COMM_WORLD);
            //for (int i = (ranks-1) * line; i < ((ranks-1) * line + line); i++) {
            for (int i = 0; i < N; i++) {
                //printf("i=%d, ranks=%d \n",i, ranks);
                    int up = i - 1 < 0 ? 0 : i - 1;
                    int down = i + 1 >= N ? i : i + 1;
                for (int j = 0; j < N; j++) {
                    int left = j - 1 < 0 ? 0 : j - 1;
                    int right = j + 1 >= N ? j : j + 1;
                    next[i][j] = (temp[i][j] + temp[up][j] + temp[down][j] + temp[i][left] + temp[i][right]) *0.2;
                    //MPI_Send(&next[i][j],1,MPI_INT,0,1,MPI_COMM_WORLD);                  
                    if (next[i][j] != temp[i][j]) balance = 0;
                }
            } 
            memcpy(temp, next, N * N * sizeof(int)); 
        }
        //MPI_Send(&next, N*N, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Isend(next, N*N, MPI_INT,0,1,MPI_COMM_WORLD, &req);
        //MPI_Reduce(next, temp, N*N, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        if(ranks==1) MPI_Send(&count,1,MPI_INT,0,2,MPI_COMM_WORLD);
    }


    //* Original Code *//

    // for (int i = 0 ; i < N; i++)
    //     for (int j = 0; j < N; j++) 
    //         temp[i][j] = random() >> 3; 


    // count = 0, balance = 0;
    // while (!balance) {
    //   count++;
    //   balance = 1;
    //   for (int i = 0; i < N; i++) {
    //     for (int j = 0; j < N; j++) {
    //       int up = i - 1 < 0 ? 0 : i - 1;
    //       int down = i + 1 >= N ? i : i + 1;
    //       int left = j - 1 < 0 ? 0 : j - 1;
    //       int right = j + 1 >= N ? j : j + 1;
    //       next[i][j] = (temp[i][j] + temp[up][j] + temp[down][j] + temp[i][left] + temp[i][right]) / 5;
    //       if (next[i][j] != temp[i][j]) balance = 0;
    //     }
    //   } 
    //   memcpy(temp, next, N * N * sizeof(int)); 
    // }

        // for (int i = 0 ; i < N; i++)
        //     for (int j = 0; j < N; j++) 
        //         printf("next[%d][%d]= %d \n",i,j, next[i][j]);

        // for (int i = 0 ; i < N; i++)
        //     for (int j = 0; j < N; j++) 
        //         printf("temp[%d][%d]= %d \n",i,j, temp[i][j]);



    MPI_Finalize();

	printf("Size: %d*%d, Seed: %d, ", N, N, seed);
	printf("Iteration: %d, Temp: %d\n", count, temp[0][0]);

    t2 = clock();
    printf("Spent Time = %lf, Rank= %d\n", (t2-t1)/(double)(CLOCKS_PER_SEC), ranks);

    return 0;
}