#ifndef MATPROD_EQUATION_H
#define MATPROD_EQUATION_H

struct matprod_equation {
    int n;
    double *A;
    double *B;
    double *C;
};

void matprod_equation_free(struct matprod_equation *eq);

#endif // MATPROD_EQUATION_H
