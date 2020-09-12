#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv)
{
  int numproces,rank,source,avg_rows, extra_rows;
  int N, seed;
  N = atoi(argv[1]);
  seed = atoi(argv[2]);
  srand(seed);
  int temp[N][N], next[N][N], current[N][N];

    clock_t t1, t2;
    t1 = clock();

   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
          temp[i][j] = random() >> 3; // avoid overflow
      }
  }

  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numproces);
  MPI_Request req;

  avg_rows = N/numproces;
  extra_rows = N%numproces;

  int count = 0, balance, all_balance = 1;
  int offset[numproces], row_counts[numproces];
  int offset_temp = 0;

  for (int i=0; i<numproces && offset_temp <= N; i++) {
      if(i == 0) {
          row_counts[i] = (extra_rows != 0)? avg_rows +1:avg_rows;
      } else {
          row_counts[i] = (i < extra_rows)? avg_rows +1:avg_rows;
      }
      offset[i] = offset_temp;
      offset_temp = offset_temp + row_counts[i];
  }

  while (all_balance) {
    count++;
    balance = 0;
    all_balance = 0;
    if (rank == 0) { /* master */
        for (int i = 0; i < row_counts[rank]; i++) {
            int up = i - 1 < 0 ? 0 : i - 1;
            int down = i + 1 >= N ? i : i + 1;
            for (int j = 0; j < N; j++) {
                int left = j - 1 < 0 ? 0 : j - 1;
                int right = j + 1 >= N ? j : j + 1;
                next[i][j] = (temp[i][j] + temp[up][j] + temp[down][j] + temp[i][left] + temp[i][right]) / 5;
                if (next[i][j] != temp[i][j]) {
                    balance = 1;
                }
            }
        }
        for (int i = 1; i < numproces; i++) {
            MPI_Send(&temp, N*N, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        for (int i = 1; i < numproces; i++) {
            source = i;
            MPI_Recv(&next[offset[i]][0], row_counts[i]*N, MPI_INT, source, 2, MPI_COMM_WORLD, &status);

        }
        memcpy(temp, next, N * N * sizeof(int));
    }
    /* slave */
    if (rank > 0) {
        source = 0;
        MPI_Recv(&temp, N*N, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        int end_row = offset[rank] + row_counts[rank];
        for (int i = offset[rank]; i < end_row; i++) {
            int up = i - 1 < 0 ? 0 : i - 1;
            int down = i + 1 >= N ? i : i + 1;
            for (int j = 0; j < N; j++) {
                int left = j - 1 < 0 ? 0 : j - 1;
                int right = j + 1 >= N ? j : j + 1;
                next[i][j] = (temp[i][j] + temp[up][j] + temp[down][j] + temp[i][left] + temp[i][right]) / 5;
                if (next[i][j] != temp[i][j]) {
                    balance = 1;
                }
            }
        }
        MPI_Send(&next[offset[rank]][0], row_counts[rank]*N, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
    MPI_Allreduce(&balance, &all_balance, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  if(rank == 0) {
    printf("Size: %d*%d, Seed: %d, ", N, N, seed);
    printf("Iteration: %d, Temp: %d\n", count, temp[0][0]);
  }
    
  return 0;
}
