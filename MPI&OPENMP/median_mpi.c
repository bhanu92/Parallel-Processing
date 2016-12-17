#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <sys/time.h>

#define MASTER 1
#define SLAVE 2
#define rows 128
#define columns 128

int main ( int argc, char** argv ) {
	int m = rows, n = columns;
	int i, j, k, l, p, q, r, s, x = 0, y = 0, z, g, t = 0;
	int a[m][n], c[(m - 2) * (n - 2)][9], b[9], d[(m - 2) * (n - 2)];
	int temp;
	int ij = 0, ik = 0, il;

	int rowsPerSlave, reminder, rowNum, rowsSend;
	int num_proc, num_slaves, proc_id;
	int sender, receiver, tag;
	struct timeval start_time, end_time;
	//double start, end, communication = 0, computation = 0;
	gettimeofday(&start_time, NULL);

	srand ( time(NULL) );

	for (i = 0; i < ((m - 2) * (n - 2)); i++) {
		d[i] = 0;
	}

	MPI_Status status;
	MPI_Init ( &argc, &argv );
	MPI_Comm_rank ( MPI_COMM_WORLD, &proc_id );
	MPI_Comm_size ( MPI_COMM_WORLD, &num_proc );
	num_slaves = num_proc - 1;

	if (proc_id == 0) {

		for ( i = 0; i < m; i++ ) {
			printf ( "\n" );

			for ( j = 0; j < n; j++ ) {
				a[i][j] = 1 + (rand() % 9);
				printf ( "%d   ", a[i][j] );
			}
		}

		for (i = 0; i < (m - 2) * (n - 2) ; i++) {
			for (j = 0; j < 9; j++) {
				c[i][j] = 0;
			}
		}

		printf("\n");
		for (i = 0; i < 9; i++) {
			b[i] = 0;
		}

		for ( i = 1; i < (m - 1) ; i++ ) {
			for ( j = 1; j < n - 1; j++ ) {
				x = 0;
				for ( k = i - 1; k < i + 2; k++ ) {
					for ( l = j - 1; l < j + 2; l++ ) {
						b[x] = a[k][l];
						c[ik][x] = b[x];
						x++;
					}
				}
				ik++;
			}
		}

		rowsPerSlave = ((m - 2) * (n - 2)) / num_slaves;
		reminder = ((m - 2) * (n - 2)) % num_slaves;
		rowNum = 0;
		for ( receiver = 1; receiver <= num_slaves; receiver++ )
		{
			if ( reminder > 0 ) {
				rowsSend = rowsPerSlave + 1;
				reminder--;
			}

			else {
				rowsSend = rowsPerSlave;
			}

			MPI_Send ( &rowNum, 1, MPI_INT, receiver, MASTER, MPI_COMM_WORLD );
			MPI_Send ( &rowsSend, 1, MPI_INT, receiver, MASTER, MPI_COMM_WORLD );
			MPI_Send ( &c[rowNum][0], rowsSend * 9, MPI_INT, receiver, MASTER, MPI_COMM_WORLD );
			rowNum = rowNum + rowsSend;
		}

		for ( sender = 1; sender <= num_slaves; sender++ ) {
			MPI_Recv ( &rowNum, 1, MPI_INT, sender, SLAVE, MPI_COMM_WORLD, &status );
			MPI_Recv ( &rowsSend, 1, MPI_INT, sender, SLAVE, MPI_COMM_WORLD, &status );
			MPI_Recv ( &d[rowNum], rowsSend, MPI_INT, sender, SLAVE, MPI_COMM_WORLD, &status );
		}


		printf ( "\nThe resultant matrix after applying median filter...\n" );
		k = 0;
		for ( i = 1; i < m - 1; i++) {
			for (j = 1; j < m - 1; j++) {
				a[i][j] = d[k];
				k++;
			}
		}
		printf("\n\n");

		for (i = 0; i < m; i++) {
			for (j = 0; j < m; j++) {
				printf("%d   ", a[i][j]);
			}
			printf("\n");
		}

		printf("\n");
	}


	if (proc_id > 0) {

		MPI_Recv ( &rowNum, 1, MPI_INT, 0, MASTER, MPI_COMM_WORLD, &status );
		MPI_Recv ( &rowsSend, 1, MPI_INT, 0, MASTER, MPI_COMM_WORLD, &status );
		MPI_Recv ( &c, rowsSend * 9, MPI_INT, 0, MASTER, MPI_COMM_WORLD, &status );

		int f, s, t = 0;
		int g;
		g = rowsSend;

		int e[rowsSend * 9];



		for ( r = 0; r < rowsSend; r++) {
			for (s = 0; s < 9; s++) {
				e[t] = c[r][s];
				t = t + 1;
			}
		}

		for (f = 0; f < (rowsSend * 9); f = f + 9) {
			for ( p = f; p < f + 9; p++ ) {
				for ( q = p + 1; q < f + 9; q++ ) {
					if ( e[q] < e[p] ) {
						temp = e[p];
						e[p] = e[q];
						e[q] = temp;
					}
				}
			}
			d[rowNum + rowsSend - g] = e[f + 4];
			//printf("%d, pid@%d\t", e[f+4],proc_id);
			//printf("Value:%d, RowNum:%d\t", d[rowNum + rowsSend - g], (rowNum + rowsSend - g));
			g--;
		}


		MPI_Send ( &rowNum, 1, MPI_INT, 0, SLAVE, MPI_COMM_WORLD );
		MPI_Send ( &rowsSend, 1, MPI_INT, 0, SLAVE, MPI_COMM_WORLD );
		MPI_Send ( &d[rowNum], rowsSend, MPI_INT, 0, SLAVE, MPI_COMM_WORLD );
	}

	MPI_Finalize();

	gettimeofday(&end_time, NULL);
	printf("\n\nTotal time taken in MIcro Seconds......%ld\n", ((end_time.tv_sec * 1000000 + end_time.tv_usec ) - (start_time.tv_sec * 1000000 + start_time.tv_usec )));
}

