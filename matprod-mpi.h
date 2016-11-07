#ifndef MATPROD_MPI_H
#define MATPROD_MPI_H

#include <mpi.h>

struct matprod_proc {
    int rank;
    int group_size;
    int N; /*  == group_length == sqrt(group_size) */
    MPI_Comm cart_comm, line_comm, col_comm;
    int line, col;
};

void matprod_mpi_init(struct matprod_proc *p);
void matprod_mpi_scatter_input(int const N, int n, double *A, double *B);
void matprod_mpi_gather_result(int const N, int n, double *C);

#endif // MATPROD_MPI_H
