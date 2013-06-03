#ifndef H_RIVER_INTERNAL_DEFINED
#define H_RIVER_INTERNAL_DEFINED

#include "river.h"

typedef struct conf Conf;
struct conf
{
    float flux;
    int   height;
    int   length;
    int   zone;
    float prob_island;
    int   freq_island;
};

List river;
TStrip base;
Conf Config;

/* Função auxiliar para imprimir
 * a faixa de terra */
void strip_print (TStrip);

#endif
