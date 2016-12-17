#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
//#include <mpi.h>

//#define MASTER 1
//#define SLAVE 2()
#define rows 8
#define columns 8

int main ( int argc, char** argv ) {
	int m = rows, n = columns;
	int i, j, k, l, p, q, x = 0;
	int a[m][n], median[m - 2][n - 2], b[9];
	int temp;
	int master;

	int thread_id, thread_num;
	srand ( time(NULL) );

	thread_num = omp_get_num_threads();
	#pragma omp parallel private(thread_id) shared(a,median,i, j, k, l, p, q, x,thread_num)
	{

		//if(thread_id == 0) thread_id = master;

		#pragma omp for schedule (static)
		for ( i = 0; i < m; i++ ) {
			printf ( "\n" );

			for ( j = 0; j < n; j++ ) {
				a[i][j] = 1 + (rand() % 10);
				printf ( "%d   ", a[i][j] );
			}
		}

		#pragma omp for schedule (static)
		for (i = 0; i < m - 2; i++) {
			for (j = 0; j < n - 2; j++) {
				median[i][j] = 0;
			}
		}
		printf("\n");

		#pragma omp for schedule (static)
		for ( i = 1; i < m - 1; i++ ) {
			for ( j = 1; j < n - 1; j++ ) {
				x = 0;
				//#pragma omp master
				for ( k = i - 1; k < i + 2; k++ ) {
					for ( l = j - 1; l < j + 2; l++ ) {
						b[x] = a[k][l];
						x++;
					}
				}
				//#pragma omp end master
				for ( p = 0; p < 9; p++ ) {
					for ( q = p + 1; q < 9; q++ ) {
						if ( b[q] < b[p] ) {
							temp = b[p];
							b[p] = b[q];
							b[q] = temp;
						}
					}
				}
				median[i - 1][j - 1] = b[x / 2];
				//printf("%d\n", b[x/2] );
			}
		}
	}


	printf ( "\n\nThe resultant matrix after applying median filter...\n\n" );

	for ( i = 1; i < m - 1; i++) {
		for (j = 1; j < m - 1; j++) {
			a[i][j] = median[i - 1][j - 1];
			//printf("%d\t",a[i][j]);
		}
		//printf("\n");
	}


	for ( i = 0; i < m; i++) {
		for (j = 0; j < m; j++) {
			printf("%d   ", a[i][j]);
		}
		printf("\n");
	}

	return 0;
}
