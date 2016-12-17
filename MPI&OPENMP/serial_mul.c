#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
	int i, j, k, l, x, y, z;
	time_t t;
	int sum = 0;
	struct timeval start, end;
	i = j = k = l = 256;
	int a[i][j], b[k][l], c[i][l] ;
	srand ( ( unsigned ) time ( &t ) );
	printf ( "\n" );

	gettimeofday(&start, NULL);

	if ( j != k ) {
		printf ( "The entered columns value of Matrix1 is not equal to entered rows value of Matrix2\n" );
		return 0;
	}

	else {
		printf ( "The Randomly generated values of First matrix are \n" );

		for ( x = 0; x < i; x++ ) {
			printf ( "\n" );

			for ( y = 0; y < j; y++ ) {
				a[x][y] = 1 + ( rand() % 10 );
				printf ( "%d   ", a[x][y] );
			}
		}

		printf ( "\n\n" );
		printf ( "The Randomly generated values of second matrix are \n" );

		for ( x = 0; x < k; x++ ) {
			printf ( "\n" );

			for ( y = 0; y < l; y++ ) {
				b[x][y] = 1 + ( rand() % 10 );
				//b[x][y] = y - x;
				printf ( "%d   ", b[x][y] );
			}
		}

		printf ( "\n" );

		for ( x = 0; x < i; x++ ) {
			for ( y = 0; y < l; y++ ) {
				c[x][y] = 0;
			}
		}

		for ( x = 0; x < i; x++ ) {
			for ( y = 0; y < l; y++ ) {
				sum = 0;

				for ( z = 0; z < j; z++ ) {
					sum += a[x][z] * b[z][y];
				}

				c[x][y] = sum;
			}
		}
	}

	printf ( "\nProduct of the two matrices:-\n\n" );

	for ( x = 0; x < i; x++ ) {
		for ( y = 0; y < l; y++ ) {
			printf ( "%d\t", c[x][y] );
		}

		printf ( "\n" );
	}

	gettimeofday(&end, NULL);

	printf("Total time taken......%ld\n", ((end.tv_sec * 1000000  + end.tv_usec) - (start.tv_sec * 1000000  + start.tv_usec)));

	return 0;
}


