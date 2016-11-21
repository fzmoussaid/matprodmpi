#ifndef MATPROD_INPUT_H
#define MATPROD_INPUT_H

double *matprod_read_input_matrix(
    int const N, int *n_output, char const *filename);
double *matprod_read_input_matrix_seq(int *n_ret, char const *filename);

#endif // MATPROD_INPUT_H

