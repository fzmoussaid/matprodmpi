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
    double min = opt.min_value_arg;
    double max = opt.max_value_arg;

    printf("%d\n", n);
    for (int j = 0 ; j < n; ++j) {
        for (int i = 0 ; i < n; ++i)
            printf("%g ", min + ((double)rand() / RAND_MAX)*(max-min));
        printf("\n");
    }
    return EXIT_SUCCESS;
}
