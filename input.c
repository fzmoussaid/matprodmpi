#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <glib.h>

#include "error.h"
#include "util.h"

static void
die_file_bad_format(char const *filename, int line)
{
    matprod_fatal("%s: line %d invalid file format\n", filename, line);
}

static int
scan_line(FILE *f, char const *format, ...)
{
    va_list ap;
    size_t len = 0;
    char *line = NULL;

    if (getline(&line, &len, f) < 0) {
        free(line);
        return -1;
    }
    va_start(ap, format);
    int n = vsscanf(line, format, ap);
    va_end(ap);
    free(line);
    return n;
}

static char *
matprod_readline(FILE *f)
{
    size_t len = 0;
    char *line = NULL;

    if (getline(&line, &len, f) < 0) {
        free(line);
        return NULL;
    }
    return g_strstrip(line);
}

double *matprod_read_input_matrix(
    int const N, int *n_ret, char const *filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    int n = 0;
    int c = scan_line(f, "%d", &n);
    if (c != 1)
        die_file_bad_format(filename, 0);

    if (n % N != 0) {
        fprintf(stderr, "erreur: La taille de la matrice n'est pas "
                "divisible par le nombre de processus");
        exit(EXIT_FAILURE);
    }
    double *mat = tdp_matrix_new(n, n);
    for (int i = 0; i < n; ++i) {
        char *line = matprod_readline(f);
        if (line == NULL)
            die_file_bad_format(filename, i+1);
        char **split = g_strsplit(line, " ", -1);
        int j = 0;
        for (int k = 0; split[k] != NULL; ++k) {
            if (!g_strcmp0(split[k], ""))
                continue;
            mat[j*n+i] = g_ascii_strtod(split[k], NULL);
            ++j;
            if (j >= n) break;
        }
        g_strfreev(split);
        if (j < n)
            die_file_bad_format(filename, i+1);
        free(line);
    }
    fclose(f);
    *n_ret = n;
    return mat;
}
