#include <stdio.h>
#include <assert.h>
#include <mpi.h>

int main(void)
{
    int size, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    assert(size == 16);

    int dims[3] = {4, 2, 2};
    int periods[3] = {1, 1, 1};
    MPI_Comm comm_cart;
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 1, &comm_cart);

    int cart_size, cart_rank;
    MPI_Comm_size(comm_cart, &cart_size);
    assert(cart_size == size);
    MPI_Comm_rank(comm_cart, &cart_rank);
    /* Get the process coordinates */
    int cart_coords[3] = {0, 0, 0};
    MPI_Cart_coords(comm_cart, cart_rank, 3, cart_coords);
    printf("cartesian rank %i has coordinates (", cart_rank);
    for (int i = 0; i < 3; ++i) {
        if (i > 0) {
            printf(", %i", cart_coords[i]);
        } else {
            printf("%i", cart_coords[i]);
        }
    }
    printf(")\n");

    MPI_Finalize();
    return 0;
}
