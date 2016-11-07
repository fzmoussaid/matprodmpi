#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "matprod-mpi.h"

void matprod_mpi_init(struct matprod_proc *p)
{

    MPI_Comm_size(MPI_COMM_WORLD, &p->group_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &p->rank);

    double Nd = sqrt(p->group_size);
    assert( ((void)"processus count must be a perfect integer square",
             floor(Nd) == Nd) );

    int N = p->N = (int)Nd;
    printf("#rank: %d; group_size: %d\n", p->rank, p->group_size);

    int dims[2] = {N, N};
    int periods[2] = {0, 1};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &p->cart_comm);

    int rem_dims[2] = {1, 0};
    MPI_Cart_sub(p->cart_comm, rem_dims, &p->line_comm);
    MPI_Comm_rank(p->line_comm, &p->line);

    int rem_dims2[2] = {0, 1};
    MPI_Cart_sub(p->cart_comm, rem_dims2, &p->col_comm);
    MPI_Comm_rank(p->col_comm, &p->col);

    printf("%d %d %d\n", p->rank, p->line, p->col);
}


static void
create_block_type(int n)
{
}

void matprod_mpi_scatter_input(int const N, int n, double *A, double *B)
{
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    create_block_type(n);
}

void matprod_mpi_gather_result(int const N, int n, double *C)
{
}
