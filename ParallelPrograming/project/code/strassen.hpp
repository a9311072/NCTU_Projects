#pragma once

#include "classical.hpp"

extern int THRESHOLD;

extern matrix_array*** M1;
extern matrix_array*** M2;
extern matrix_array*** M3;
extern matrix_array*** M4;
extern matrix_array*** M5;
extern matrix_array*** M6;
extern matrix_array*** M7;

void strassen_openmp(matrix_array*** C, matrix_array*** A, matrix_array*** B, const int& size, const int& lv)
{
	int size_2 = size/2;
	if (size <= THRESHOLD) // To prevent core dump
	{
		classical_openmp(C, A, B, size, lv);
		return;
	}


	/**** M1 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		A[lv+1][i][j] = A[lv][i][j] + A[lv][i+size_2][j+size_2];

#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		B[lv+1][i][j] = B[lv][i][j] + B[lv][i+size_2][j+size_2];

	strassen_openmp(M1, A, B, size_2, lv+1);


	/**** M2 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		A[lv+1][i][j] = A[lv][i+size_2][j] + A[lv][i+size_2][j+size_2];

#pragma omp parallel for
	for (int i = 0; i < size_2; ++i)
		copy(&B[lv][i][0], &B[lv][i][0]+size_2, &B[lv+1][i][0]);

	strassen_openmp(M2, A, B, size_2, lv+1);


	/**** M3 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i)
		copy(&A[lv][i][0], &A[lv][i][0]+size_2, &A[lv+1][i][0]);

#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		B[lv+1][i][j] = B[lv][i][j+size_2] - B[lv][i+size_2][j+size_2];

	strassen_openmp(M3, A, B, size_2, lv+1);


	/**** M4 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i)
		copy(&A[lv][i+size_2][size_2], &A[lv][i+size_2][size_2]+size_2, &A[lv+1][i][0]);

#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		B[lv+1][i][j] = B[lv][i+size_2][j] - B[lv][i][j];

	strassen_openmp(M4, A, B, size_2, lv+1);


	/**** M5 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		A[lv+1][i][j] = A[lv][i][j] + A[lv][i][j+size_2];

#pragma omp parallel for
	for (int i = 0; i < size_2; ++i)
		copy(&B[lv][i+size_2][size_2], &B[lv][i+size_2][size_2]+size_2, &B[lv+1][i][0]);

	strassen_openmp(M5, A, B, size_2, lv+1);


	/**** M6 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		A[lv+1][i][j] = A[lv][i+size_2][j] - A[lv][i][j];

#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		B[lv+1][i][j] = B[lv][i][j] + B[lv][i][j+size_2];

	strassen_openmp(M6, A, B, size_2, lv+1);


	/**** M7 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		A[lv+1][i][j] = A[lv][i][j+size_2] - A[lv][i+size_2][j+size_2];

#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		B[lv+1][i][j] = B[lv][i+size_2][j] + B[lv][i+size_2][j+size_2];

	strassen_openmp(M7, A, B, size_2, lv+1);


	/**** C1 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		C[lv][i][j] = M1[lv+1][i][j] + M4[lv+1][i][j] - M5[lv+1][i][j] + M7[lv+1][i][j];


	/**** C2 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		C[lv][i][j+size_2] = M3[lv+1][i][j] + M5[lv+1][i][j];


	/**** C3 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		C[lv][i+size_2][j] = M2[lv+1][i][j] + M4[lv+1][i][j];


	/**** C4 ****/
#pragma omp parallel for
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		C[lv][i+size_2][j+size_2] = M1[lv+1][i][j] - M2[lv+1][i][j] + M3[lv+1][i][j] + M6[lv+1][i][j];
}


void strassen_original(matrix_array*** C, matrix_array*** A, matrix_array*** B, const int& size, const int& lv)
{
	int size_2 = size/2;
	if (size <= THRESHOLD) // To prevent core dump
	{
		classical_original(C, A, B, size, lv);
		return;
	}


	/**** M1 ****/
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		A[lv+1][i][j] = A[lv][i][j] + A[lv][i+size_2][j+size_2];


	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		B[lv+1][i][j] = B[lv][i][j] + B[lv][i+size_2][j+size_2];

	strassen_original(M1, A, B, size_2, lv+1);


	/**** M2 ****/
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		A[lv+1][i][j] = A[lv][i+size_2][j] + A[lv][i+size_2][j+size_2];


	for (int i = 0; i < size_2; ++i)
		copy(&B[lv][i][0], &B[lv][i][0]+size_2, &B[lv+1][i][0]);

	strassen_original(M2, A, B, size_2, lv+1);


	/**** M3 ****/
	for (int i = 0; i < size_2; ++i)
		copy(&A[lv][i][0], &A[lv][i][0]+size_2, &A[lv+1][i][0]);


	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		B[lv+1][i][j] = B[lv][i][j+size_2] - B[lv][i+size_2][j+size_2];

	strassen_original(M3, A, B, size_2, lv+1);


	/**** M4 ****/
	for (int i = 0; i < size_2; ++i)
		copy(&A[lv][i+size_2][size_2], &A[lv][i+size_2][size_2]+size_2, &A[lv+1][i][0]);


	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		B[lv+1][i][j] = B[lv][i+size_2][j] - B[lv][i][j];

	strassen_original(M4, A, B, size_2, lv+1);


	/**** M5 ****/
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		A[lv+1][i][j] = A[lv][i][j] + A[lv][i][j+size_2];


	for (int i = 0; i < size_2; ++i)
		copy(&B[lv][i+size_2][size_2], &B[lv][i+size_2][size_2]+size_2, &B[lv+1][i][0]);

	strassen_original(M5, A, B, size_2, lv+1);


	/**** M6 ****/
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		A[lv+1][i][j] = A[lv][i+size_2][j] - A[lv][i][j];


	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		B[lv+1][i][j] = B[lv][i][j] + B[lv][i][j+size_2];

	strassen_original(M6, A, B, size_2, lv+1);


	/**** M7 ****/
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		A[lv+1][i][j] = A[lv][i][j+size_2] - A[lv][i+size_2][j+size_2];


	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		B[lv+1][i][j] = B[lv][i+size_2][j] + B[lv][i+size_2][j+size_2];

	strassen_original(M7, A, B, size_2, lv+1);


	/**** C1 ****/
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		C[lv][i][j] = M1[lv+1][i][j] + M4[lv+1][i][j] - M5[lv+1][i][j] + M7[lv+1][i][j];


	/**** C2 ****/
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		C[lv][i][j+size_2] = M3[lv+1][i][j] + M5[lv+1][i][j];


	/**** C3 ****/
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		C[lv][i+size_2][j] = M2[lv+1][i][j] + M4[lv+1][i][j];


	/**** C4 ****/
	for (int i = 0; i < size_2; ++i) for (int j = 0; j < size_2; ++j)
		C[lv][i+size_2][j+size_2] = M1[lv+1][i][j] - M2[lv+1][i][j] + M3[lv+1][i][j] + M6[lv+1][i][j];
}
