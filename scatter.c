#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>

#define DELTA 0.01

int main(void)
{
    int size, rank;
    float *sendbuf = NULL;
    float data;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* MPI_Scatter() */
    if (rank == 0) {
        sendbuf = malloc(sizeof(*sendbuf) * size);
        assert(sendbuf != NULL);
        for (int i = 0; i < size; ++i) {
            sendbuf[i] = i / 2.0;
        }
        printf("=> testing MPI_Scatter()\n");
    }
    MPI_Scatter(sendbuf, 1, MPI_FLOAT, &data, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        free(sendbuf);
    }
    printf("==> rank %i received %f\n", rank, data);

    MPI_Barrier(MPI_COMM_WORLD);

    /* MPI_Scatterv() */
    float *recvbuf = NULL;
    int *sendcounts = NULL, *displs = NULL;
    if (rank == 0) {
        sendbuf = malloc(sizeof(*sendbuf) * ((size * (size + 1)) / 2));
        assert(sendbuf != NULL);
        sendcounts = malloc(sizeof(*sendcounts) * size);
        assert(sendcounts != NULL);
        displs = malloc(sizeof(*displs) * size);
        assert(displs != NULL);
        for (int i = 0, disp = 0; i < size; ++i, disp += i + 1) {
            sendcounts[i] = i + 1;
            displs[i] = disp;
            for (int j = 0; j < (i + 1); ++j) {
                sendbuf[disp + j] = 0.3 * j;
            }
        }
        printf("=> testing MPI_Scatterv()\n");
    }
    recvbuf = malloc(sizeof(*recvbuf) * (rank + 1));
    assert(recvbuf != NULL);
    MPI_Scatterv(sendbuf, sendcounts, displs, MPI_FLOAT, recvbuf, rank + 1,
                 MPI_FLOAT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < (rank + 1); ++i) {
        assert(recvbuf[i] > (0.3 * i - DELTA) && recvbuf[i] < (0.3 * i + DELTA));
    }
    if (rank == 0) {
        printf("==> complete\n");
        free(sendbuf);
        free(sendcounts);
        free(displs);
    } else {
        free(recvbuf);
    }

    MPI_Finalize();
    return 0;
}
