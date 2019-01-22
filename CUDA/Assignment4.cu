#define BLOCK_DIM	4
#define TILE_DIM	BLOCK_DIM

#include <stdio.h>
#include <stdlib.h>
#include<time.h>

void Print_Matrix( int* mtxArray , int n, int m );
void PrintMatrixToText(int* mtxArray, int height, int width, const char* fileName);

// Matrix Mult Kernel
__global__ void matrixMult(int* A, int* B, int* C, int ARows, int ACols, int BRows, int BCols, int CRows, int CCols) {
	
	__shared__ int As[TILE_DIM][TILE_DIM];
    __shared__ int Bs[TILE_DIM][TILE_DIM];
    
    int bx = blockIdx.x;  int by = blockIdx.y;
	int tx = threadIdx.x; int ty = threadIdx.y;

	int Row = by * blockDim.y + ty;
	int Col = bx * blockDim.x + tx;
	
	int CValue = 0;
	int NUM_PHASE = (int)ceil((float)(ACols + BLOCK_DIM - 1) / (float)BLOCK_DIM);

    for (int k = 0; k < NUM_PHASE; ++k) {
			
        if (k*TILE_DIM + tx < ACols && Row < ARows)	
			As[ty][tx] = A[Row*ACols + k*TILE_DIM + tx];
			
		else													
			As[ty][tx] = 0;

		if (k*TILE_DIM + ty < BRows && Col < BCols)	
			Bs[ty][tx] = B[(k*TILE_DIM + ty)*BCols + Col];
			
		else													
			Bs[ty][tx] = 0;
         
		__syncthreads();

			for (int i = 0; i < TILE_DIM; ++i) 
				CValue += As[ty][i] * Bs[i][tx];
				
		__syncthreads();
    }
    
    if (Row < CRows && Col < CCols) 
		C[(Row*CCols)+Col] = CValue;
		
}

int main(){
	int *h_a, *h_b, *h_c;	// Host Variables
	int *d_a, *d_b, *d_c;	// Device Variables
	int n,m,k;
	
	// Receive Parameters
	/*Todo:Write Receive Parameters through console commands*/
	printf("n = ");
	scanf("%d", &n);
	printf("m = ");
	scanf("%d", &m);
	printf("k = ");
	scanf("%d", &k);
	printf("\n");
	
	// Alloc & Initialize Host Input Matrices	
	int sizeA = (n) * (m) * sizeof(int);
	int sizeB = (m) * (k) * sizeof(int);
	int sizeC = (n) * (k) * sizeof(int);
	h_a = (int*)malloc( sizeA );
	h_b = (int*)malloc( sizeB );
	h_c = (int*)malloc( sizeC );
	
	/*Todo: Initialize A and B with Random Varialbles Ranged (-10, 10)*/
	srand(time(NULL));
		
	for ( int i=0 ; i<n ; i++ )
		for ( int j=0 ; j<m ; j++ )
			h_a[i*m+j] = rand() % 20 - 10;
		
	for ( int i=0 ; i<m ; i++ )
		for ( int j=0 ; j<k ; j++ )
			h_b[i*k+j] = rand() % 20 - 10;

	// Allocate Device Memory
	cudaMalloc((void **) &d_a, sizeA);
	cudaMalloc((void **) &d_b, sizeB);
	cudaMalloc((void **) &d_c, sizeC);

	// Copy Input Matrices to Device Memory
	cudaMemcpy(d_a, h_a, sizeA, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, h_b, sizeB, cudaMemcpyHostToDevice);

	
	// Set Grid/Block Dimensions
	dim3 dimGrid((int)ceil((float)(BLOCK_DIM+n-1)/(float)BLOCK_DIM), (int)ceil((float)(BLOCK_DIM+k-1)/(float)BLOCK_DIM));
	dim3 dimBlock(BLOCK_DIM,BLOCK_DIM);

	// Launch Kernel
	matrixMult<<<dimGrid ,dimBlock >>>(d_a , d_b , d_c , n, m, m, k, n, k);

	// Copy Result to Host
	cudaMemcpy( h_c, d_c, sizeC, cudaMemcpyDeviceToHost );
	
	// Free Device Memory
	cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
	
	// Print Results
	printf("[Input matrix A]\n");	Print_Matrix( h_a , n, m );
	printf("[Input matrix B]\n");	Print_Matrix( h_b , m, k );
	
	printf("[GPU Result]\n");	Print_Matrix( h_c , n, k );
	
	// Print Results
	PrintMatrixToText(h_a,n,m,"matrixA.txt");
	PrintMatrixToText(h_b,m,k,"matrixB.txt");
	PrintMatrixToText(h_c,n,k,"matrixC.txt");
	
	// Free Host Memory
	free(h_a); free(h_b); free(h_c);
	
	return 0;
}

// Utilities
void Print_Matrix( int* mtxArray , int n, int m )
{
	for ( int i=0 ; i<n ; i++ )
	{
		for ( int j=0 ; j<m ; j++ )
		{
			printf( "%d " , mtxArray[i*m+j] );
		}
		printf( "\n" );
	}	
	printf( "\n" );
}

void PrintMatrixToText(int* mtxArray, int height, int width, const char* fileName){
	FILE *f = fopen(fileName, "w");
	for ( int i=0 ; i<height ; i++ )
	{
		for ( int j=0 ; j<width ; j++ )
		{
			fprintf(f, "%d\t" , mtxArray[i*width+j] );
		}
		fprintf(f,"\n" );
	}	
	
	fclose(f);
}

