#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "matprod-mpi.h"
#include "fox_mult.h"
#include "util.h"

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

    {
        int rem_dims[2] = {1, 0};
        MPI_Cart_sub(p->cart_comm, rem_dims, &p->line_comm);
        MPI_Comm_rank(p->line_comm, &p->col);
    }

    {
        int rem_dims[2] = {0, 1};
        MPI_Cart_sub(p->cart_comm, rem_dims, &p->col_comm);
        MPI_Comm_rank(p->col_comm, &p->line);
    }

    printf("R=%d L=%d C=%d\n", p->rank, p->line, p->col);

    int garbage = p->col;
    MPI_Cart_shift(p->col_comm, 0, 1, &garbage, &p->next_col);
    MPI_Cart_shift(p->col_comm, 0, -1, &garbage, &p->prev_col);
}

MPI_Datatype column_type;
MPI_Datatype block_type;

static void
create_block_type(int const b, int const N)
{
    MPI_Type_contiguous(b, MPI_DOUBLE, &column_type);
    MPI_Type_commit(&column_type);

    MPI_Datatype t;
    MPI_Type_vector(b, 1, N, column_type, &t);
    MPI_Aint lb, extent, extentCol;
    MPI_Type_get_extent(column_type, &lb, &extentCol);
    MPI_Type_get_extent(t, &lb, &extent);

    MPI_Type_create_resized(t, lb, extentCol, &block_type);

    MPI_Type_commit(&block_type);
}

void matprod_mpi_scatter_input(struct matprod_proc const *p,
                               struct matprod_equation const *root_eq,
                               struct matprod_equation *local_eq)
{
    int const N = p->N;
    int b = root_eq->n / N;

    MPI_Bcast(&b, 1, MPI_INT, 0, MPI_COMM_WORLD);
    create_block_type(b, N);

    local_eq->n = b;
    local_eq->A = tdp_matrix_new(b, b);
    local_eq->B = tdp_matrix_new(b, b);
    local_eq->C = tdp_matrix_new(b, b);

    int *sendcnts = NULL, *disps = NULL;

    if (!p->rank) {
        sendcnts = calloc(p->group_size, sizeof*sendcnts);
        disps = calloc(p->group_size, sizeof*disps);

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int coord[2] = { j, i };
                int rank;
                MPI_Cart_rank(p->cart_comm, coord, &rank);
                sendcnts[rank] = 1;
                disps[rank] = j*N*b+i;
            }
        }
    }

    MPI_Scatterv(root_eq->A, sendcnts, disps, block_type,
                 local_eq->A, b, column_type, 0, p->cart_comm);
    MPI_Scatterv(root_eq->B, sendcnts, disps, block_type,
                 local_eq->B, b, column_type, 0, p->cart_comm);
    if (!p->rank) {
        free(sendcnts);
        free(disps);
    }
}

void matprod_mpi_gather_result(struct matprod_proc const *p,
                               struct matprod_equation const *root_eq,
                               struct matprod_equation *local_eq)
{
    int const N = p->N;
    int const b = local_eq->n;
    int *recvcnts = NULL, *disps = NULL;

    if (!p->rank) {
        recvcnts = malloc(p->group_size * sizeof*recvcnts);
        disps = malloc(p->group_size * sizeof*disps);

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int coord[2] = { j, i };
                int rank;
                MPI_Cart_rank(p->cart_comm, coord, &rank);
                recvcnts[rank] = 1;
                disps[rank] = j*N*b+i;
            }
        }
    }

    MPI_Gatherv(local_eq->C, b, column_type,
                root_eq->C, recvcnts, disps, block_type, 0, p->cart_comm);
    if (!p->rank) {
        free(recvcnts);
        free(disps);
    }
}
