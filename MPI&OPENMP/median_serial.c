#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define rows 8
#define columns 8

int main ( int argc, char** argv ) {
	int m = rows, n = columns;
	int i, j, k, l, p, q, x = 0;
	int a[m][n], median[m - 2][n - 2], b[9];
	int temp;
	struct timeval start, end;
	srand ( time(NULL) );

	gettimeofday(&start, NULL);

	for ( i = 0; i < m; i++ ) {
		printf ( "\n" );

		for ( j = 0; j < n; j++ ) {
			a[i][j] = 1 + (rand() % 10);
			printf ( "%d   ", a[i][j] );
		}
	}
	for (i = 0; i < m - 2; i++) {
		for (j = 0; j < n - 2; j++) {
			median[i][j] = 0;
		}
	}
	printf("\n");
	for ( i = 1; i < m - 1; i++ ) {
		for ( j = 1; j < n - 1; j++ ) {
			x = 0;
			for ( k = i - 1; k < i + 2; k++ ) {
				for ( l = j - 1; l < j + 2; l++ ) {
					b[x] = a[k][l];
					x++;
				}
			}

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

	gettimeofday(&end, NULL);

	printf("Total time taken......%ld\n", ((end.tv_sec * 1000000  + end.tv_usec) - (start.tv_sec * 1000000  + start.tv_usec)));

	return 0;
}
