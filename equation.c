#include <stdlib.h>
#include <string.h>

#include "equation.h"

void matprod_equation_free(struct matprod_equation *eq)
{
    free(eq->A);
    free(eq->B);
    free(eq->C);
    memset(eq, 0, sizeof*eq);
}
