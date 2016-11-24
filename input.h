#ifndef MATPROD_INPUT_H
#define MATPROD_INPUT_H

#include "equation.h"

void matprod_read_input_matrices(
    int N, struct matprod_equation *eq, char *filenames[], bool binary);

#endif // MATPROD_INPUT_H

