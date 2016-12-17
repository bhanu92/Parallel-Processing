#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
	int i, j, k, l, x, y, z;
	time_t t;
	int thread_id, thread_num;
	int sum = 0;
	double start, totalTime;
	/*printf ( "Enter the value for number of rows in A: " );
	scanf ( "%d", &i );
	printf ( "Enter the value for number of columns in A: " );
	scanf ( "%d", &j );
	printf ( "Enter the value for number of rows in B: " );
	scanf ( "%d", &k );
	printf ( "Enter the value for number of columns in B: " );
	scanf ( "%d", &l );*/
	i = j = k = l = 256;
	int a[i][j], b[k][l], c[i][l] ;
	srand ( ( unsigned ) time ( &t ) );
	printf ( "\n" );
	thread_num = omp_get_num_threads();
	start = omp_get_wtime();
	#pragma omp parallel private(thread_id,x,y,z) shared(a,b,c,i,j,k,l,thread_num)
	{
		thread_id = omp_get_thread_num();
		printf ( "Thread %d is ready\n", thread_id );
		#pragma omp barrier

		if ( thread_id == 0 ) {
			thread_num = omp_get_num_threads();
			printf ( "All the %d threads are ready for matrix multiplication\n", thread_num );
		}

		#pragma omp barrier
		printf ( "\nThe Randomly generated values of First matrix are \n" );
		#pragma omp for schedule (static)

		for ( x = 0; x < i; x++ ) {
			printf ( "\n" );

			for ( y = 0; y < j; y++ ) {
				a[x][y] = 1 + ( rand() % 10 );
				printf ( "%d   ", a[x][y] );
			}
		}

		#pragma omp barrier
		printf ( "\n\n" );
		printf ( "\nThe Randomly generated values of second matrix are \n" );
		#pragma omp for schedule (static)

		for ( x = 0; x < k; x++ ) {
			printf ( "\n" );

			for ( y = 0; y < l; y++ ) {
				b[x][y] = 1 + ( rand() % 10 );
				//b[x][y] = y - x;
				printf ( "%d   ", b[x][y] );
			}
		}

		#pragma omp barrier
		printf ( "\n" );
		#pragma omp for schedule (static)

		for ( x = 0; x < i; x++ ) {
			for ( y = 0; y < l; y++ ) {
				c[x][y] = 0;
			}
		}

		printf ( "Thread %d starting matrix multiply...\n", thread_id );
		#pragma omp barrier
		#pragma omp for schedule (static)
		for ( x = 0; x < i; x++ ) {
			printf ( "Thread=%d is computing row %d of Matrix C\n", thread_id, x );

			for ( y = 0; y < l; y++ ) {
				sum = 0;

				for ( z = 0; z < j; z++ ) {
					sum += a[x][z] * b[z][y];
				}

				c[x][y] = sum;
			}
		}
	}

	totalTime = omp_get_wtime() - start;

	printf ( "\nProduct of the two matrices:-\n\n" );

	for ( x = 0; x < i; x++ ) {
		for ( y = 0; y < l; y++ ) {
			printf ( "%d\t", c[x][y] );
		}

		printf ( "\n" );
	}

	printf("Total Time taken in Micro seconds.....%lf", 1000000 * totalTime);

	return 0;
}


