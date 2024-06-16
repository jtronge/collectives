#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>

int main(void)
{
    int size, rank;
    float *sendbuf = NULL;
    float data;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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

    MPI_Finalize();
    return 0;
}
