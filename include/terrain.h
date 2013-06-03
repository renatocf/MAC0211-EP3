#ifndef H_TERRAIN_DEFINED
#define H_TERRAIN_DEFINED

#define LAND  '#'
#define WATER '.'

/* Tipo para terreno: struct com
 * campos de VELOCIDADE (v) e TIPO
 * DE TERRENO (t) */
struct _terrain
{
    float v;
    char  t;
};
typedef struct _terrain terrain;

#endif
