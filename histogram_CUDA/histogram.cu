#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda.h>

#define ROWS 4096
#define COLS 4096

__global__ void histo(int* d_hist1, int* d_hist2, int* mat) {

	int id;
	id = blockIdx.x * blockDim.x + threadIdx.x;

	switch (d_hist1[id]) {
	case 0:
		atomicAdd(&mat[0], 1);
		break;
	case 1:
		atomicAdd(&mat[1], 1);
		break;
	case 2:
		atomicAdd(&mat[2], 1);
		break;
	case 3:
		atomicAdd(&mat[3], 1);
		break;
	case 4:
		atomicAdd(&mat[4], 1);
		break;
	case 5:
		atomicAdd(&mat[5], 1);
		break;
	case 6:
		atomicAdd(&mat[6], 1);
		break;
	case 7:
		atomicAdd(&mat[7], 1);
		break;
	case 8:
		atomicAdd(&mat[8], 1);
		break;
	case 9:
		atomicAdd(&mat[9], 1);
		break;

	}
	__syncthreads();
	switch (d_hist2[id]) {
	case 0:
		atomicAdd(&mat[0], 1);
		break;
	case 1:
		atomicAdd(&mat[1], 1);
		break;
	case 2:
		atomicAdd(&mat[2], 1);
		break;
	case 3:
		atomicAdd(&mat[3], 1);
		break;
	case 4:
		atomicAdd(&mat[4], 1);
		break;
	case 5:
		atomicAdd(&mat[5], 1);
		break;
	case 6:
		atomicAdd(&mat[6], 1);
		break;
	case 7:
		atomicAdd(&mat[7], 1);
		break;
	case 8:
		atomicAdd(&mat[8], 1);
		break;
	case 9:
		atomicAdd(&mat[9], 1);
		break;


	}

}

int main(int argc, char* argv[]) {

	//int histogram[ROWS][COLS];
	int x, y;
	int hist[ROWS * COLS], hist1[(ROWS * COLS) / 2], hist2[(ROWS * COLS) / 2];
	int *d_hist1, *d_hist2;
	int i = 0;
	int mat[10];
	int *d_mat;


	FILE *fp;
	fp = fopen("input.txt", "r");
	for (x = 0; x < ROWS; x++)
	{
		for ( y = 0; y < COLS; y++)
		{
			fscanf(fp, "%d", &hist[i]);
			// printf("%d\t",hist[i]);
			i++;
		}

	}
	fclose(fp);

	printf("\n");

	//int i = 0;
	/*for (x = 0; x < ROWS; x++) {
			for (y = 0; y < COLS; y++) {
					hist[i] = histogram[x][y];
			}
	}*/

	for (i = 0; i < (ROWS * COLS) / 2; i++) {
		hist1[i] = hist[i];
		//	printf("%d\t", hist[i]);
	}
	printf("\n");
	for (i = 0; i < (ROWS * COLS) / 2; i++) {
		hist2[i] = hist[((ROWS * COLS) / 2) + i];
		//	printf("%d\t", hist[i]);
	}

	for (i = 0; i < 10; i++) {
		mat[i] = 0;
	}

	cudaMalloc(&d_hist1, ROWS * COLS * sizeof(int) / 2);
	cudaMalloc(&d_hist2, ROWS * COLS * sizeof(int) / 2);
	cudaMalloc(&d_mat, 10 * sizeof(int));


	cudaMemcpy(d_hist1, hist1, (ROWS * COLS * sizeof(int)) / 2, cudaMemcpyHostToDevice);
	cudaMemcpy(d_hist2, hist2, (ROWS * COLS * sizeof(int)) / 2, cudaMemcpyHostToDevice);
	cudaMemcpy(d_mat, &mat, 10 * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemset(d_mat, 0, sizeof(d_mat));


	//histo <<<16384,1024>>>(d_hist, d_mat, ROWS * COLS);
	histo <<< 8192, 1024>>>(d_hist1, d_hist2, d_mat);


	cudaThreadSynchronize();

	cudaMemcpy(mat, d_mat, 10 * sizeof(int), cudaMemcpyDeviceToHost);

	FILE *fout;
	fout = fopen("output.txt", "w");

	for (i = 0; i < 10; i++) {
		printf("\n%d => %d", i, mat[i]);
		fprintf(fout, "%d => %d\n", i, mat[i]);
	}

	fclose(fout);
	cudaFree(d_mat);
	cudaFree(d_hist1);
	cudaFree(d_hist2);
	return 0;

}
