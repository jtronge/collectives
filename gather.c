#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

int main(void)
{
    int size, rank;
    double *recvbuf = NULL;
    double tmp;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    assert(size >= 4);

    /* Try MPI_Gather() first */
    if (rank == 0) {
        recvbuf = malloc(sizeof(*recvbuf) * size);
        assert(recvbuf != NULL);
        printf("=> running MPI_Gather()\n");
    }
    tmp = (double) rank / size;
    /* recvbuf is significant at specified root only */
    MPI_Gather(&tmp, 1, MPI_DOUBLE, recvbuf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("==> data received = [%g, %g, %g, ...]\n", recvbuf[0], recvbuf[1],
               recvbuf[2]);
        free(recvbuf);
    }

    /* Now run MPI_Gatherv() */
    int *sendbuf;
    sendbuf = malloc(sizeof(*sendbuf) * (rank + 1));
    assert(sendbuf != NULL);
    /* Each rank sends (rank + 1) data (all random) */
    for (int i = 0; i < (rank + 1); ++i) {
        sendbuf[i] = rank + i;
    }

    if (rank == 0) {
        int *counts = NULL, *disps = NULL, *recvbuf = NULL;
        int total_rcount = 0;

        counts = malloc(sizeof(*counts) * size);
        assert(counts != NULL);
        disps = malloc(sizeof(*disps) * size);
        assert(disps != NULL);
        for (int i = 0; i < size; ++i) {
            counts[i] = i + 1;
            disps[i] = i > 0 ? disps[i-1] + counts[i-1] : 0;
            total_rcount += counts[i];
        }

        recvbuf = malloc(sizeof(*recvbuf) * total_rcount);
        assert(recvbuf != NULL);
        printf("=> running MPI_Gatherv()\n");
        MPI_Gatherv(sendbuf, 1, MPI_INT, recvbuf, counts, disps, MPI_INT, 0, MPI_COMM_WORLD);
        for (int other_rank = 0; other_rank < size; ++other_rank) {
            for (int i = 0; i < (other_rank + 1); ++i) {
                assert(recvbuf[disps[other_rank] + i] == other_rank + i);
            }
        }
        printf("==> check complete\n");

        free(recvbuf);
        free(disps);
        free(counts);
    } else {
        MPI_Gatherv(sendbuf, rank + 1, MPI_INT, NULL, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);
    }

    free(sendbuf);
    MPI_Finalize();
    return 0;
}
