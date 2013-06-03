#ifndef H_STRIP_DEFINED
#define H_STRIP_DEFINED

#include "terrain.h"

/* Critério utilizado para declarar que o
 * terreno não será baseado em nenhum outro */
#define NO_BASE     NULL

/* Faixa de terreno: vetor de terrenos */
typedef terrain *TStrip;

/* Funções para manipular faixas de terreno */
void   tstrip_seed     (int);
void   tstrip_free     (TStrip);
void   tstrip_print    (TStrip);
void   tstrip_island   (float prob, int freq);
TStrip tstrip_generate (int size, int zone,
                        float normalization, TStrip base, TStrip nova);

#endif
