#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>

#include "cmdline.h"
#include "error.h"
#include "../util.h"

int main(int argc, char *argv[])
{
    srand((long) &argc + time(NULL));

    struct gengetopt_args_info opt;

    cmdline_parser(argc, argv, &opt);
    int n = opt.size_arg;

    double *M = tdp_matrix_new(n, n);
    tdp_matrix_rand(n, n, M, opt.min_value_arg, opt.max_value_arg);

    printf("%d\n", n);
    tdp_matrix_print(n, n, M, n, stdout);
    free(M);

    return EXIT_SUCCESS;
}
