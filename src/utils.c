#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

void *mallocSafe(size_t n)
{
    void *pt;
    pt = malloc(n);
    if (pt == NULL)
    {
        printf("ERRO na alocacao de memoria.\n\n");
        exit(-1);
    }
    return pt;
}
