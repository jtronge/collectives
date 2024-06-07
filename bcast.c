#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

int main(void)
{
    int size, rank;
    int *data;
    int idata[4];
    MPI_Request reqs[4];

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    data = malloc(sizeof(*data) * 64);
    if (rank == 0) {
        /* Rank 0 sets the data */
        for (int i = 0; i < 64; ++i) {
            data[i] = i;
        }
    }
    /* Data is broadcast from rank 0 to all other ranks */
    MPI_Bcast(data, 64, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) {
        printf("rank %i received [%i, %i, %i, %i, ...]\n", rank, data[0],
               data[1], data[2], data[3]);
    }
    free(data);

    /* Now try a non-blocking broadcast */
    if (rank == 0) {
        idata[0] = 33;
        idata[1] = 28;
        idata[2] = -2323;
        idata[3] = 0;
    }
    for (int i = 0; i < 4; ++i) {
        MPI_Ibcast(&idata[i], 1, MPI_INT, 0, MPI_COMM_WORLD, &reqs[i]);
    }
    if (rank > 1) {
        sleep(2);
    }
    MPI_Waitall(4, reqs, MPI_STATUSES_IGNORE);
    if (rank != 0) {
        printf("rank %i received [%i, %i, %i, %i] (non-blocking bcast)\n", rank, idata[0], idata[1],
               idata[2], idata[3]);
    }

    MPI_Finalize();
    return 0;
}
