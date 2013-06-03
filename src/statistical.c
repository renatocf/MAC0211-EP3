#include <stdlib.h>
#include <math.h>
#include "statistical.h"

void stat_set_seed(int seed)
{
    srand(seed);
}
/* Gera float entre dois inteiros */

float stat_gen_uniform_float(float min, float max)
{
    float d;
    d = (float) rand() / ((float) RAND_MAX + 1);
    return (min + d * (max - min));
}

int stat_gen_uniform_int(int min, int max)
{
    int k;
    double d;
    d = (double) rand() / ((double) RAND_MAX + 1);
    k = (int) (d * (max - min + 1));
    return (min + k);
}


