#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <assert.h>


#include "cmdline.h"
#include "error.h"
#include "perf/perf.h"

int main(int argc, char *argv[])
{
    int group_size, rank;
    struct gengetopt_args_info opt;

    cmdline_parser(argc, argv, &opt);
    if (!opt.inputs_num) {
        cmdline_parser_print_help();
        matprod_error("No input file!\n");
        exit(EXIT_FAILURE);
    }
    double *mat;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &group_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double Nd = sqrt(group_size);
    assert(floor(Nd) == Nd);
    int N = (int)Nd;
    printf("#rank: %d; group_size: %d\n", rank, group_size);
    
    MPI_Comm cart, line, column;
    int dims[2] = {N, N};
    int periods[2] = {0, 1};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart);

    int rank_line, rank_column;
    int rem_dims[2] = {1, 0};
    MPI_Cart_sub(cart, rem_dims, &line);
    MPI_Comm_rank(line, &rank_line);

    int rem_dims2[2] = {0, 1};
    MPI_Cart_sub(cart, rem_dims2, &column);
    MPI_Comm_rank(column, &rank_column);

    printf("%d %d %d\n",rank,rank_line, rank_column);

    perf_t p1, p2;
    perf(&p1);
    // do work
    perf(&p2);
    perf_diff(&p1, &p2);

    uint64_t micro, max_t;
    micro = perf_get_micro(&p2);
    MPI_Reduce(&micro, &max_t, 1, MPI_UNSIGNED_LONG,
               MPI_MAX, 0, MPI_COMM_WORLD);
    if (!rank)
        fprintf(stderr, "time: %lu.%06lu s\n", max_t/1000000UL, max_t%1000000UL);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
