#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

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
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &group_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("#rank: %d; group_size: %d\n", rank, group_size);

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
