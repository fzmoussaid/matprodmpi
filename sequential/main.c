#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "../cmdline.h"
#include "../cblas.h"
#include "../error.h"
#include "../util.h"
#include "../input.h"
#include "../perf/perf.h"
#include "../fox_mult.h"

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

static inline void print_time(void)
{
    uint64_t micro;
    micro = perf_get_micro(&p2);
    fprintf(stderr, "time: %lu µs\n", micro);
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
    struct matprod_equation eq;
    memset(&eq, 0, sizeof eq);

    matprod_read_input_matrices(-1, &eq, opt.inputs, opt.binary_flag);

    time_start();
    int N = eq.n;
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans,
                N, N, N, 1.0, eq.A, N, eq.B, N, 0.0, eq.C, N);
    time_end();

    if (opt.print_flag) {
        printf("N=%d\n", eq.n);
        tdp_matrix_print(eq.n, eq.n, eq.C, eq.n, stdout);
    }
    matprod_equation_free(&eq);
    print_time();
    return EXIT_SUCCESS;
}
