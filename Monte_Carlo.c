#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

void main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int rank;
	int np;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	MPI_Status status;

	long N = 10000000;
	int radius = 1;

	int count = N / np;
	int offset = N % np;

	srand(time(NULL) * rank);

	if(rank == 0) {

		double start_time = MPI_Wtime();
		int amountSent;
		double x, y;
		int totalNoOfPointsInsideCircle = 0;
		int noOfPointsInsideCircle = 0;
		int numReceived;
		for(int i = 1; i < np; i++) {
			if(i == np - 1) {
				amountSent = count + offset;
				MPI_Send(&amountSent, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
			else {
				amountSent = count;
				MPI_Send(&amountSent, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
		}

		for(int i = 0; i < count; i++) {
			x = rand() % 1000 / 1000.0;
			y = rand() % 1000 / 1000.0;
			if(pow(x, 2) + pow(y, 2) <= pow(radius, 2)) {
				noOfPointsInsideCircle++;
			}
		}

		totalNoOfPointsInsideCircle = totalNoOfPointsInsideCircle + noOfPointsInsideCircle;

		for(int i = 1; i < np; i++) {
			MPI_Recv(&numReceived, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			totalNoOfPointsInsideCircle = totalNoOfPointsInsideCircle + numReceived;
		}
		printf("PI: %f\n", ((totalNoOfPointsInsideCircle / (double)N) * 4));
		double end_time = MPI_Wtime();
		printf("Total time taken: %f\n", end_time - start_time);
	}
	else {
		int numgen;
		int noOfPointsInsideCircle = 0;
		MPI_Recv(&numgen, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		double x, y;
		for(int i = 0; i < numgen; i++) {
			x = rand() % 1000 / 1000.0;
			y = rand() % 1000 / 1000.0;
			if(pow(x, 2) + pow(y, 2) <= pow(radius, 2)) {
				noOfPointsInsideCircle++;
			}
		}
		MPI_Send(&noOfPointsInsideCircle, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
