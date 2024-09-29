#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>

#define BUFFER_SIZE 128

int main(void)
{
    int size, rank;
    int sendbuf[BUFFER_SIZE];
    int recvbuf[BUFFER_SIZE];
    int recvcounts[4] = {64, 32, 16, 16};

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    assert(size == 4);

    for (int i = 0; i < BUFFER_SIZE; ++i) {
        sendbuf[i] = rank;
    }
    MPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, MPI_INT, MPI_SUM,
                       MPI_COMM_WORLD);
    for (int i = 0; i < recvcounts[rank]; ++i) {
        /* recvbuf[i] == 0 + 1 + 2 + 3 (== sum of ranks) */
        assert(recvbuf[i] == 6);
    }

    MPI_Finalize();
    return 0;
}
