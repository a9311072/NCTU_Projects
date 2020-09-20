#pragma once


void classical_openmp_sum(matrix_array*** C, matrix_array*** A, matrix_array*** B, const int& size, const int& lv)
{
#pragma omp parallel for
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			matrix_array sum = 0;
			for (int k = 0; k < size; ++k) {
				sum += A[lv][i][k] * B[lv][k][k];
			}
			C[lv][i][j] = sum;
		}
	}
}


void classical_openmp(matrix_array*** C, matrix_array*** A, matrix_array*** B, const int& size, const int& lv)
{
#pragma omp parallel for
	for (int i = 0; i < size; ++i) {
		fill(&C[lv][i][0], &C[lv][i][0]+size, 0);

		for (int k = 0; k < size; ++k)
			for (int j = 0; j < size; ++j)
				C[lv][i][j] += A[lv][i][k] * B[lv][k][j];
	}
}

void classical_original(matrix_array*** C, matrix_array*** A, matrix_array*** B, const int& size, const int& lv)
{
	for (int i = 0; i < size; ++i) {
		fill(&C[lv][i][0], &C[lv][i][0]+size, 0);

		for (int k = 0; k < size; ++k)
			for (int j = 0; j < size; ++j)
				C[lv][i][j] += A[lv][i][k] * B[lv][k][j];
	}
}
