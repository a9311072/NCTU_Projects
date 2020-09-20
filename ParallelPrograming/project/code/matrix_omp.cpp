// g++ -O2 -std=c++11 strassen_omp.cpp -o strassen_omp -Xpreprocessor -fopenmp -lomp
// ./strassen_omp <size> <threshold>

#define MAX_DEPTH 20  // Requirement: 2^MAX_DEPTH > SIZE

#define MOD 10

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <omp.h>
using namespace std;

typedef double matrix_array;

#include "strassen.hpp"
#include "classical.hpp"
#include "utils.hpp"


matrix_array*** A;
matrix_array*** B;
matrix_array*** C;

matrix_array*** M1;
matrix_array*** M2;
matrix_array*** M3;
matrix_array*** M4;
matrix_array*** M5;
matrix_array*** M6;
matrix_array*** M7;


int SIZE;
int THRESHOLD;

int N_WARMUP = 5;
int N_TEST = 50;


void init_memory()
{
	int N = SIZE;

	A = new matrix_array**[MAX_DEPTH];
	B = new matrix_array**[MAX_DEPTH];
	C = new matrix_array**[MAX_DEPTH];

	M1 = new matrix_array**[MAX_DEPTH];
	M2 = new matrix_array**[MAX_DEPTH];
	M3 = new matrix_array**[MAX_DEPTH];
	M4 = new matrix_array**[MAX_DEPTH];
	M5 = new matrix_array**[MAX_DEPTH];
	M6 = new matrix_array**[MAX_DEPTH];
	M7 = new matrix_array**[MAX_DEPTH];

	for (int i = 0; i < MAX_DEPTH && N > 0; ++i)
	{
		A[i] = new matrix_array*[N];
		B[i] = new matrix_array*[N];

		for (int j = 0; j < N; ++j)
		{
			A[i][j] = new matrix_array[N];
			B[i][j] = new matrix_array[N];
		}

		if (i == 0) {
			C[i] = new matrix_array*[N];

			for (int j = 0; j < N; ++j)
			{
				C[i][j] = new matrix_array[N];
			}
		}

		else 
		{  // i >= 1
			M1[i] = new matrix_array*[N];
			M2[i] = new matrix_array*[N];
			M3[i] = new matrix_array*[N];
			M4[i] = new matrix_array*[N];
			M5[i] = new matrix_array*[N];
			M6[i] = new matrix_array*[N];
			M7[i] = new matrix_array*[N];

			for (int j = 0; j < N; ++j)
			{
				M1[i][j] = new matrix_array[N];
				M2[i][j] = new matrix_array[N];
				M3[i][j] = new matrix_array[N];
				M4[i][j] = new matrix_array[N];
				M5[i][j] = new matrix_array[N];
				M6[i][j] = new matrix_array[N];
				M7[i][j] = new matrix_array[N];
			}
		}

		N /= 2;
	}
}

void reset()
{
	int N = SIZE;

	for (int i = 0; i < MAX_DEPTH && N > 0; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			for (int k = 0; k < N; ++k)
			{
				A[i][j][k] = (matrix_array) (rand() % MOD - (MOD/2));
				B[i][j][k] = (matrix_array) (rand() % MOD - (MOD/2));

				if (i == 0) {
					C[i][j][k] = 0;
				}
				else {  // i >= 1
					M7[i][j][k] = M6[i][j][k] = M5[i][j][k] = M4[i][j][k] = M3[i][j][k] = M2[i][j][k] = M1[i][j][k] = 0;
				}
			}
		}

		N /= 2;
	}
}


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Usage: %s <N> <threshold>\n", argv[0]);
		exit(0);
	}

	SIZE = atoi(argv[1]);
	THRESHOLD = atoi(argv[2]);

	double time_spent;

	init_memory();
	srand(time(0));
    printf("start to run [%d, %d] \n", SIZE, THRESHOLD);

	/* Test classical original */
	time_spent = 0;
	reset();
	classical_original(C, A, B, SIZE, 0);

    start_time();
	for (int cnt = 0; cnt < N_TEST; ++cnt)
	{
		reset();
		classical_original(C, A, B, SIZE, 0);
	}
    end_time();
    time_spent += get_elapsed_time();
	printf("[classical_original] Spent time : %.10lfs\n", time_spent);

	/* Test classical omp */
	time_spent = 0;
	reset();
	classical_openmp(C, A, B, SIZE, 0);

    start_time();
	for (int cnt = 0; cnt < N_TEST; ++cnt)
	{
		reset();
		classical_openmp(C, A, B, SIZE, 0);
	}
    end_time();
    time_spent += get_elapsed_time();
	printf("[classical_omp] Spent time : %.10lfs\n", time_spent);


	/* Test strassen_original */
    time_spent = 0;
    reset();
    strassen_original(C, A, B, SIZE, 0);

    start_time();
	for (int cnt = 0; cnt < N_TEST; ++cnt)
	{
		reset();
		strassen_original(C, A, B, SIZE, 0);
	}
    end_time();
    time_spent += get_elapsed_time();
    printf("[strassen_original] Spent time : %.10lfs\n", time_spent);

	/* Test strassen_omp */
	time_spent = 0;
	reset();
	strassen_openmp(C, A, B, SIZE, 0);

    start_time();
	for (int cnt = 0; cnt < N_TEST; ++cnt)
	{
		reset();
		strassen_openmp(C, A, B, SIZE, 0);
	}
    end_time();
    time_spent += get_elapsed_time();
	printf("[strassen_omp]  Spent time : %.10lfs\n\n", time_spent);


	return 0;
}