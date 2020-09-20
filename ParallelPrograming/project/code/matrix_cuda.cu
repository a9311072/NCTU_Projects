//  nvcc -arch=sm_50 strassen-cuda_v1.cu -o strassen-cuda_v1 -lcublas 
//  bash ./mybash
//
#include <cuda_runtime.h>
#include <cublas_v2.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#define BLOCK_SIZE 16
#define N_TEST 50
#define MAX_DEPTH 20


float *temp_A, *temp_B, *mul_Array;
float *mat_A[MAX_DEPTH], *mat_B[MAX_DEPTH], *mat_C[MAX_DEPTH];
float *M1[MAX_DEPTH], *M2[MAX_DEPTH], *M3[MAX_DEPTH], *M4[MAX_DEPTH], *d_M5[MAX_DEPTH], *M6[MAX_DEPTH], *M7[MAX_DEPTH];


template <typename matrixArray>
void fillMatrix(matrixArray* arr, const int N)
{
	for (int i = 0; i < N; ++i)
	{
		arr[i] = (matrixArray) (rand() & 0xF);
	}
}

template <typename matrixArray>
__global__
void classicalMatmul(matrixArray* A, matrixArray* B, matrixArray* C, const int dim)
{
	const int row = blockIdx.y * blockDim.y + threadIdx.y;
	const int col = blockIdx.x * blockDim.x + threadIdx.x;
	const int gd = gridDim.x;

	__shared__ matrixArray _A[BLOCK_SIZE][BLOCK_SIZE], _B[BLOCK_SIZE][BLOCK_SIZE];

	if (row < dim && col < dim)
	{
		matrixArray sum = 0;
		for (int k = 0; k < gd; ++k)
		{
			_A[threadIdx.y][threadIdx.x] = A[row*dim + k*BLOCK_SIZE + threadIdx.x];
			_B[threadIdx.y][threadIdx.x] = B[(k*BLOCK_SIZE+threadIdx.y) * dim + col];
			__syncthreads();

			for (int l = 0; l < BLOCK_SIZE; ++l)
			{
				sum += _A[threadIdx.y][l] * _B[l][threadIdx.x];
			}
			__syncthreads();
		}

		C[row*dim + col] = sum;
	}
}


template <typename matrixArray>
void strassenMatmul(cublasHandle_t& handle, matrixArray* A, matrixArray* B, matrixArray* C, const int dim, const int d, const int threshold)
{
	const int dim_2 = dim/2;

	int lda = dim, ldb = dim, ldc = dim_2;
	int m = dim_2, n = dim_2;
	matrixArray one = 1, zero = 0, m_one = -1;

	if (dim <= threshold)
	{
		dim3 block(BLOCK_SIZE, BLOCK_SIZE);
		dim3 grid((dim+BLOCK_SIZE-1)/BLOCK_SIZE, (dim+BLOCK_SIZE-1)/BLOCK_SIZE);
		classicalMatmul<matrixArray><<< grid, block >>>(A, B, C, dim);
		// cublasSgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, dim, dim, dim, &one, B, dim, A, dim, &zero, C, dim);
		return;
	}


	/* M1 */
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, A, lda, &one, A+dim_2*dim+dim_2, ldb, mat_A[d+1], ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, B, lda, &one, B+dim_2*dim+dim_2, ldb, mat_B[d+1], ldc);
	strassenMatmul(handle, mat_A[d+1], mat_B[d+1], M1[d+1], dim_2, d+1, threshold);

	/* M2 */
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, A+dim_2*dim, lda, &one, A+dim_2*dim+dim_2, ldb, mat_A[d+1], ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, B, lda, &zero, B, ldb, mat_B[d+1], ldc);
	strassenMatmul(handle, mat_A[d+1], mat_B[d+1], M2[d+1], dim_2, d+1, threshold);

	/* M3 */
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, A, lda, &zero, A, ldb, mat_A[d+1], ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, B+dim_2, lda, &m_one, B+dim_2*dim+dim_2, ldb, mat_B[d+1], ldc);
	strassenMatmul(handle, mat_A[d+1], mat_B[d+1], M3[d+1], dim_2, d+1, threshold);

	/* M4 */
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, A+dim_2*dim+dim_2, lda, &zero, A, ldb, mat_A[d+1], ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, B+dim_2*dim, lda, &m_one, B, ldb, mat_B[d+1], ldc);
	strassenMatmul(handle, mat_A[d+1], mat_B[d+1], M4[d+1], dim_2, d+1, threshold);

	/* M5 */
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, A, lda, &one, A+dim_2, ldb, mat_A[d+1], ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, B+dim_2*dim+dim_2, lda, &zero, B, ldb, mat_B[d+1], ldc);
	strassenMatmul(handle, mat_A[d+1], mat_B[d+1], d_M5[d+1], dim_2, d+1, threshold);

	/* M6 */
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, A+dim_2*dim, lda, &m_one, A, ldb, mat_A[d+1], ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, B, lda, &one, B+dim_2, ldb, mat_B[d+1], ldc);
	strassenMatmul(handle, mat_A[d+1], mat_B[d+1], M6[d+1], dim_2, d+1, threshold);

	/* M7 */
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, A+dim_2, lda, &m_one, A+dim_2*dim+dim_2, ldb, mat_A[d+1], ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, B+dim_2*dim, lda, &one, B+dim_2*dim+dim_2, ldb, mat_B[d+1], ldc);
	strassenMatmul(handle, mat_A[d+1], mat_B[d+1], M7[d+1], dim_2, d+1, threshold);


	/* C1 */
	lda = dim, ldb = dim/2, ldc = dim;  // C = C + B
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &zero, C, lda, &one, M1[d+1], ldb, C, ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, C, lda, &one, M4[d+1], ldb, C, ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, C, lda, &m_one, d_M5[d+1], ldb, C, ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, C, lda, &one, M7[d+1], ldb, C, ldc);

	/* C2 */
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &zero, C+dim_2, lda, &one, M3[d+1], ldb, C+dim_2, ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, C+dim_2, lda, &one, d_M5[d+1], ldb, C+dim_2, ldc);

	/* C3 */
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &zero, C+dim_2*dim, lda, &one, M2[d+1], ldb, C+dim_2*dim, ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, C+dim_2*dim, lda, &one, M4[d+1], ldb, C+dim_2*dim, ldc);

	/* C4 */
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &zero, C+dim_2*dim+dim_2, lda, &one, M1[d+1], ldb, C+dim_2*dim+dim_2, ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, C+dim_2*dim+dim_2, lda, &m_one, M2[d+1], ldb, C+dim_2*dim+dim_2, ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, C+dim_2*dim+dim_2, lda, &one, M3[d+1], ldb, C+dim_2*dim+dim_2, ldc);
	cublasSgeam(handle, CUBLAS_OP_N, CUBLAS_OP_N, m, n, &one, C+dim_2*dim+dim_2, lda, &one, M6[d+1], ldb, C+dim_2*dim+dim_2, ldc);
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Usage: %s <size> <threshold>\n", argv[0]);
		exit(0);
	}

    clock_t t1, t2;
	/* Initialize */

	int n_size = atoi(argv[1]);
	int threshold = atoi(argv[2]);
	//int check = atoi(argv[3]);
	
	printf("[size= %d, threshold = %d] \n", n_size, threshold );

	dim3 block(BLOCK_SIZE, BLOCK_SIZE);
	dim3 grid((n_size+BLOCK_SIZE-1)/BLOCK_SIZE, (n_size+BLOCK_SIZE-1)/BLOCK_SIZE);

	assert(n_size >= threshold && threshold >= BLOCK_SIZE);

	size_t nBytes = n_size * n_size * sizeof(float);

	temp_A = (float*) malloc(nBytes);
	temp_B = (float*) malloc(nBytes);
	mul_Array = (float*) malloc(nBytes);

	srand(0);
	fillMatrix<float>(temp_A, n_size*n_size);
	fillMatrix<float>(temp_B, n_size*n_size);

	int depth, _dim = n_size;
	for (depth = 0; depth < MAX_DEPTH && _dim > 0; ++depth)
	{
		cudaMalloc((float**) &mat_A[depth], _dim*_dim*sizeof(float));
		cudaMalloc((float**) &mat_B[depth], _dim*_dim*sizeof(float));

		if (depth == 0)
		{
			cudaMalloc((float**) &mat_C[depth], _dim*_dim*sizeof(float));
		}
		else
		{
			cudaMalloc((float**) &M1[depth], _dim*_dim*sizeof(float));
			cudaMalloc((float**) &M2[depth], _dim*_dim*sizeof(float));
			cudaMalloc((float**) &M3[depth], _dim*_dim*sizeof(float));
			cudaMalloc((float**) &M4[depth], _dim*_dim*sizeof(float));
			cudaMalloc((float**) &d_M5[depth], _dim*_dim*sizeof(float));
			cudaMalloc((float**) &M6[depth], _dim*_dim*sizeof(float));
			cudaMalloc((float**) &M7[depth], _dim*_dim*sizeof(float));
		}
		_dim /= 2;
	}

	cudaMemcpy(mat_A[0], temp_A, nBytes, cudaMemcpyHostToDevice);
	cudaMemcpy(mat_B[0], temp_B, nBytes, cudaMemcpyHostToDevice);

	cublasHandle_t handle;
	cublasCreate(&handle);


	/* Run classicalMatmul */
	t1 = clock();
	for (int i = 0; i < N_TEST; ++i)
	{
		classicalMatmul<float><<< grid, block >>>(mat_A[0], mat_B[0], mat_C[0], n_size);
		cudaDeviceSynchronize();
	}
	t2 = clock();
	printf("[classicalMatmul] %lfs \n", (t2-t1)/(double)(CLOCKS_PER_SEC));


	/* Run strassenMatmul */
	t1 = clock();
	for (int i = 0; i < N_TEST; ++i)
	{
		strassenMatmul<float>(handle, mat_A[0], mat_B[0], mat_C[0], n_size, 0, threshold);
	}
	t2 = clock();
	printf("[strassenMatmul] %lfs \n\n", (t2-t1)/(double)(CLOCKS_PER_SEC));
	cudaMemcpy(mul_Array, mat_C[0], nBytes, cudaMemcpyDeviceToHost);


	/* Free memory */
	cublasDestroy(handle);

	for (int i = 0; i < depth; ++i)
	{
		cudaFree(mat_A[i]);
		cudaFree(mat_B[i]);

		if (i == 0)
		{
			cudaFree(mat_C[i]);
		}
		else
		{
			cudaFree(M1[i]);
			cudaFree(M2[i]);
			cudaFree(M3[i]);
			cudaFree(M4[i]);
			cudaFree(d_M5[i]);
			cudaFree(M6[i]);
			cudaFree(M7[i]);
		}
	}

	cudaDeviceReset();

	free(temp_A);
	free(temp_B);
	free(mul_Array);

	return 0;
}