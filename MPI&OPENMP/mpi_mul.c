#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>

#define MASTER 1
#define SLAVE 2

int main ( int argc, char** argv ) {
	int num_proc;
	int num_slaves;
	int proc_id;
	int sender, receiver;
	int rowsPerSlave, reminder, rowsSend;
	int rowNum;
	int tag;
	struct timeval start_time, end_time;
	int i, j, k, l, x, y, z;
	i = j = k = l = 128;
	time_t t;
	int a[i][j], b[k][l], c[i][l] ;
	srand ( ( unsigned ) time ( &t ) );
	double start, end, communication = 0, computation = 0;

	gettimeofday(&start_time, NULL);

	MPI_Status status;
	MPI_Init ( &argc, &argv );
	MPI_Comm_rank ( MPI_COMM_WORLD, &proc_id );
	MPI_Comm_size ( MPI_COMM_WORLD, &num_proc );
	num_slaves = num_proc - 1;


	MPI_Barrier(MPI_COMM_WORLD);

	start = MPI_Wtime();
	if ( proc_id == 0 ) {

		printf ( "The Randomly generated values of First matrix are \n" );

		for ( x = 0; x < i; x++ ) {
			printf ( "\n" );

			for ( y = 0; y < j; y++ ) {
				a[x][y] = 1 + ( rand() % 10 );
				//a[x][y]=1;
				printf ( "%d   ", a[x][y] );
			}
		}

		printf ( "\n\n" );
		printf ( "The Randomly generated values of second matrix are \n" );

		for ( x = 0; x < k; x++ ) {
			printf ( "\n" );

			for ( y = 0; y < l; y++ ) {
				b[x][y] = 1 + ( rand() % 10 );
				printf ( "%d   ", b[x][y] );
			}
		}

		printf ( "\n" );

		for ( x = 0; x < i; x++ ) {
			for ( y = 0; y < l; y++ ) {
				c[x][y] = 0;
			}
		}
	}
	end = MPI_Wtime();
	computation += end - start;

	start = MPI_Wtime();

	MPI_Bcast(b, k * l, MPI_INT, 0, MPI_COMM_WORLD);
	end = MPI_Wtime();
	communication = communication + (end - start);
	MPI_Barrier(MPI_COMM_WORLD);
	rowsPerSlave = i / num_slaves;
	reminder = i % num_slaves;
	rowNum = 0;
	if (proc_id == 0) {
		for ( receiver = 1; receiver <= num_slaves; receiver++ ) {
			if ( reminder > 0 ) {
				rowsSend = rowsPerSlave + 1;
				reminder--;
			}

			else {
				rowsSend = rowsPerSlave;
			}

			printf ( "\nNumber of Rows of Matrix A sent to Slave %d : %d", receiver, rowsSend );
			/*  int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest,

			        int tag, MPI_Comm comm) */
			start = MPI_Wtime();
			MPI_Send ( &rowNum, 1, MPI_INT, receiver, MASTER, MPI_COMM_WORLD );
			MPI_Send ( &rowsSend, 1, MPI_INT, receiver, MASTER, MPI_COMM_WORLD );
			MPI_Send ( &a[rowNum][0], rowsSend * j, MPI_INT, receiver, MASTER, MPI_COMM_WORLD );
			end = MPI_Wtime();
			communication += end - start;
			rowNum = rowNum + rowsSend;
		}
	}


	if ( proc_id > 0 ) {
		/*  int MPI_Recv(void *buf, int count, MPI_Datatype datatype,
		    int source, int tag, MPI_Comm comm, MPI_Status *status)*/
		//start = MPI_Wtime();
		MPI_Recv ( &rowNum, 1, MPI_INT, 0, MASTER, MPI_COMM_WORLD, &status );
		MPI_Recv ( &rowsSend, 1, MPI_INT, 0, MASTER, MPI_COMM_WORLD, &status );
		MPI_Recv ( &a, rowsSend * j, MPI_INT, 0, MASTER, MPI_COMM_WORLD, &status );
		end = MPI_Wtime();
		communication += end - start;

		start  = MPI_Wtime();
		for ( x = 0; x < i; x++ ) {
			for ( y = 0; y < l; y++ ) {
				c[x][y] = 0;
			}
		}

		for ( x = 0; x < l; x++ ) {
			for ( y = 0; y < rowsSend; y++ ) {
				for ( z = 0; z < j; z++ ) {
					c[y][x] += a[y][z] * b[z][x];
				}
			}
		}
		end = MPI_Wtime();
		computation += end - start;

		start = MPI_Wtime();
		MPI_Send ( &rowNum, 1, MPI_INT, 0, SLAVE, MPI_COMM_WORLD );
		MPI_Send ( &rowsSend, 1, MPI_INT, 0, SLAVE, MPI_COMM_WORLD );
		MPI_Send ( &c, rowsSend * l, MPI_INT, 0, SLAVE, MPI_COMM_WORLD );
		end = MPI_Wtime();
		communication += end - start;
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (proc_id == 0) {
		for ( sender = 1; sender <= num_slaves; sender++ ) {
			start = MPI_Wtime();
			MPI_Recv ( &rowNum, 1, MPI_INT, sender, SLAVE, MPI_COMM_WORLD, &status );
			MPI_Recv ( &rowsSend, 1, MPI_INT, sender, SLAVE, MPI_COMM_WORLD, &status );
			MPI_Recv ( &c[rowNum][0], rowsSend * l, MPI_INT, sender, SLAVE, MPI_COMM_WORLD, &status );
			end = MPI_Wtime();
			communication += end - start;
		}

		printf ( "\n\nProduct of the two matrices:-\n\n" );

		for ( x = 0; x < i; x++ ) {
			for ( y = 0; y < l; y++ ) {
				printf ( "%d\t", c[x][y] );
			}

			printf ( "\n" );
		}
	}

	MPI_Finalize();

	gettimeofday(&end_time, NULL);
	printf("\ncomputaion Value in micro seconds......%lf", computation * 1000000);
	printf("\ncommunication Value micro seconds......%lf", communication * 1000000);
	printf("\n\nTotal time taken in MIcro Seconds......%ld\n", ((end_time.tv_sec * 1000000 + end_time.tv_usec ) - (start_time.tv_sec * 1000000 + start_time.tv_usec )));

}















