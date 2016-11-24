#ifndef MATPROD_MPI_H
#define MATPROD_MPI_H

#include <mpi.h>

struct matprod_proc;
extern MPI_Datatype column_type;
extern MPI_Datatype block_type;

#include "equation.h"

struct matprod_proc {
    int rank;
    int group_size;
    int N; /*  == group_length == sqrt(group_size) */
    MPI_Comm cart_comm, line_comm, col_comm;
    int line, col;
    int prev_col, next_col;
};

void matprod_mpi_init(struct matprod_proc *p);
void matprod_mpi_scatter_input(struct matprod_proc const *proc,
                               struct matprod_equation const *eq,
                               struct matprod_equation *local_eq);
void matprod_mpi_gather_result(struct matprod_proc const *proc,
                               struct matprod_equation const *eq,
                               struct matprod_equation *local_eq);

#endif // MATPROD_MPI_H
