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
        fprintf(stderr, "time: %lu.%06lu s\n",
                max_t/1000000UL, max_t%1000000UL);
}

static void
fox_algorithm(struct matprod_proc *p, struct matprod_equation *root_eq)
{
    struct matprod_equation eq;
    memset(&eq, 0, sizeof eq);

    matprod_mpi_scatter_input(p, root_eq, &eq);
    matprod_perform_fox_mult(p, &eq);
    matprod_mpi_gather_result(p, root_eq, &eq);
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

    struct matprod_equation eq;
    memset(&eq, 0, sizeof eq);

    if (!p.rank) {
        int m;
        eq.A = matprod_read_input_matrix(p.N, &eq.n, opt.inputs[0]);
        eq.B = matprod_read_input_matrix(p.N, &m, opt.inputs[1]);
        assert( ((void)"matrix input must have same size", eq.n == m) );
        eq.C = tdp_matrix_new(m, m);
    }

    time_start();
    fox_algorithm(&p, &eq);
    time_end();

    if (!p.rank)
        tdp_matrix_print(eq.n, eq.n, eq.C, eq.n, stdout);
    matprod_equation_free(&eq);

    print_time(p.rank);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
