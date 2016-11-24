#ifndef MATPROD_FOX_MULT_H
#define MATPROD_FOX_MULT_H

#include "matprod-mpi.h"
#include "equation.h"

void matprod_perform_fox_mult(struct matprod_proc *proc,
                              struct matprod_equation *eq);

#endif // MATPROD_FOX_MULT_H
