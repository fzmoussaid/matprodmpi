#ifndef MATPROD_FOX_MULT_H
#define MATPROD_FOX_MULT_H

struct matprod_equation;

#include "matprod-mpi.h"

struct matprod_equation {
    int n;
    double *A;
    double *B;
    double *C;
};

void matprod_perform_fox_mult(struct matprod_proc *proc,
                              struct matprod_equation *eq);
void matprod_equation_free(struct matprod_equation *eq);

#endif // MATPROD_FOX_MULT_H
