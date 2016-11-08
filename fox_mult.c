#include <stdlib.h>
#include <string.h>

#include "cblas.h"
#include "fox_mult.h"
#include "util.h"

void matprod_perform_fox_mult(struct matprod_proc *p,
                              struct matprod_equation *eq)
{
    int const N = p->N;
    int const i = p->line;
    int const j = p->col;
    int const b = eq->n; // block size

    double *tmp = tdp_matrix_new(b, b);
    double *Aptr = NULL;

    for (int k = 0; k < N; ++k) {
        Aptr = (j == (i+k)%N) ? eq->A : tmp;
        MPI_Bcast(Aptr, b, column_type, (i+k)%N, p->line_comm);

        cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
                    b, b, b, 1.0, Aptr, b, eq->B, b, 1.0*(k!=0),
                    eq->C, b);
        if (k == N-1) break;
        MPI_Status status;
        MPI_Sendrecv_replace(eq->B, b, column_type,
                             p->prev_col, 0, p->next_col, 0,
                             p->col_comm, &status);
    }
    free(tmp);
}

void matprod_equation_free(struct matprod_equation *eq)
{
    free(eq->A);
    free(eq->B);
    free(eq->C);
    memset(eq, 0, sizeof*eq);
}
