#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <assert.h>

#include "cmdline.h"
#include "error.h"
#include "util.h"
#include "input.h"
#include "fox_mult.h"
#include "matprod-mpi.h"
#include "perf/perf.h"

perf_t p1, p2;
static inline void
time_start(void)
{
    perf(&p1);
}

static inline void
time_end(void)
{
    perf(&p2);
    perf_diff(&p1, &p2);
}

static inline void
print_time(int rank)
{
    uint64_t micro, max_t;
    micro = perf_get_micro(&p2);
    MPI_Reduce(&micro, &max_t, 1,
               MPI_UNSIGNED_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
    if (!rank)
        fprintf(stderr, "time: %lu.%06lu s\n", max_t/1000000UL, max_t%1000000UL);
}

static void
fox_algorithm(struct matprod_proc *p, int n, double *A, double *B, double *C)
{
    matprod_mpi_scatter_input(p->N, n, A, B);
    matprod_perform_fox_mult(p->N, n, A, B, C);
    matprod_mpi_gather_result(p->N, n, C);
}

int main(int argc, char *argv[])
{
    struct gengetopt_args_info opt;

    cmdline_parser(argc, argv, &opt);
    if (opt.inputs_num < 2) {
        cmdline_parser_print_help();
        matprod_error("Missing input file(s)!\n");
        exit(EXIT_FAILURE);
    }

    struct matprod_proc p;
    MPI_Init(NULL, NULL);
    matprod_mpi_init(&p);

    int n = 0;
    double *A = NULL, *B = NULL, *C = NULL;
    if (!p.rank) {
        int m;
        A = matprod_read_input_matrix(p.N, &n, opt.inputs[0]);
        B = matprod_read_input_matrix(p.N, &m, opt.inputs[1]);
        assert( ((void)"matrix input must have same size", n == m) );
        C = tdp_matrix_new(n, n);
    }

    time_start();
    fox_algorithm(&p, n, A, B, C);
    time_end();

    print_time(p.rank);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
