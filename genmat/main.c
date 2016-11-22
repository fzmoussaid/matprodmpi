#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cmdline.h"
#include "error.h"
#include "../util.h"


void print_text_matrix(int n, double min, double max)
{
    printf("%d\n", n);
    for (int j = 0 ; j < n; ++j) {
        for (int i = 0 ; i < n; ++i)
            printf("%g ", min + ((double)rand() / RAND_MAX)*(max-min));
        printf("\n");
    }
}

void print_binary_matrix(int n, double min, double max, bool force)
{
    struct stat st;
    fstat(STDOUT_FILENO, &st);
    if (S_ISCHR(st.st_mode) && !force) {
        fprintf(stderr, "\tWarning: stdout seems to be a terminal and "
                "output is in binary mode.\n\tUse the -f option if this is "
                "really what you want\n");
        exit(EXIT_FAILURE);
    }

    fwrite(&n, sizeof n, 1, stdout);
    for (int i = 0 ; i < n*n; ++i) {
        double d = min + ((double)rand() / RAND_MAX)*(max-min);
        fwrite(&d, sizeof d, 1, stdout);
    }
}

int main(int argc, char *argv[])
{
    srand((long) &argc + time(NULL));
    struct gengetopt_args_info opt;

    cmdline_parser(argc, argv, &opt);
    int n = opt.size_arg;
    double min = opt.min_value_arg;
    double max = opt.max_value_arg;

    if (opt.binary_flag)
        print_binary_matrix(n, min, max, opt.force_flag != 0);
    else
        print_text_matrix(n, min, max);
    fclose(stdout);

    return EXIT_SUCCESS;
}
