#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "mpi.h"



int main(int argc, char **argv) {
  int N, seed;
  N = atoi(argv[1]);
  seed = atoi(argv[2]);
  srand(seed);

  int temp[N][N];

  clock_t t1, t2, t3, t4;
  t1 = clock();

    int count = 0, balance = 0;
    int next[N][N];

  int ranks, ntasks;
  MPI_Request req;
  MPI_Status status;

  MPI_Init( &argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &ranks);
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

  int x=0, y=0;
  // if(ranks==0) {
    for (x = 0; x < N; x++) {
      for (y = 0; y < N; y++) {
        temp[x][y] = random() >> 3; // avoid overflow
        MPI_Isend(&temp[x][y], 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &req);  
      } 
    } 
  // }
  // else if (ranks==1){
  //   MPI_Recv(&temp[x][y], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
  //   MPI_Send(&temp, N*N, MPI_INT, 2, 0, MPI_COMM_WORLD);
  // }
  // else if(ranks==2){
  //   MPI_Recv(&temp, N*N, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
    
    MPI_Allreduce(temp, next, N, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    count = 0, balance = 0;
    // int next[N][N];
    while (!balance) {
      count++;
      balance = 1;
      //for (int i = 0; i < N; i++){
        //MPI_Recv(&temp, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
        //MPI_Irecv(&temp[i], N, MPI_INT, 0, 0, MPI_COMM_WORLD, &req); 
      //}
      //MPI_Wait(&req, &status);
      for (int i = 0; i < N; i++) {
        //MPI_Irecv(&temp[i][0], N, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
        //MPI_Recv(&temp[i][0], N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
        //MPI_Wait(&req, &status);
        for (int j = 0; j < N; j++) {
          int up = i - 1 < 0 ? 0 : i - 1;
          int down = i + 1 >= N ? i : i + 1;
          int left = j - 1 < 0 ? 0 : j - 1;
          int right = j + 1 >= N ? j : j + 1;
          //printf("up= %d, down= %d, left= %d, right= %d \n",up,down,left,right);
          next[i][j] = (temp[i][j] + temp[up][j] + temp[down][j] +
              temp[i][left] + temp[i][right]) / 5;
          if (next[i][j] != temp[i][j]) balance = 0;

          //MPI_Allreduce(&next[i][j], &temp[i][j], 1,MPI_INT, MPI_SUM, MPI_COMM_WORLD );
        }
        //MPI_Irecv(&temp[i][0], N, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
      }
      //MPI_Irecv(temp, N, MPI_INT, 0, 0, MPI_COMM_WORLD, &req); 
      //MPI_Recv(temp, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
      memcpy(temp, next, N * N * sizeof(int)); 
      
    }

  // }

  //MPI_Recv(temp, N, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
  //MPI_Type_free(&rowtype);
  MPI_Finalize();

  printf("Size: %d*%d, Seed: %d, ", N, N, seed);
  printf("Iteration: %d, Temp: %d, Rank %d\n", count, temp[0][0],ranks);
  //printf("Rank %d out of %d processors\n", ranks, ntasks);
  t2 = clock();
  //printf("Spent Time = %lf\n", (t2-t1)/(double)(CLOCKS_PER_SEC));
  return 0;
}

