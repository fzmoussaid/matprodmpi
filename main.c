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

static inline void
print_time(int rank, double time)
{
    double max_time;
    MPI_Reduce(&time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (!rank)
        fprintf(stderr, "time: %g s\n", max_time);
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

    if (!p.rank)
        matprod_read_input_matrices(p.N, &eq, opt.inputs, opt.binary_flag);

    double time = MPI_Wtime();
    fox_algorithm(&p, &eq);
    time = MPI_Wtime() - time;

    if (!p.rank && opt.print_flag)
        tdp_matrix_print(eq.n, eq.n, eq.C, eq.n, stdout);
    matprod_equation_free(&eq);

    print_time(p.rank, time);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
