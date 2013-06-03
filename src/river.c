/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                              BIBLIOTECAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
/* Bibliotecas-padrão */
#include <stdio.h>
#include <stdlib.h>

/* bibliotecas internas */
#include "list.h"
#include "utils.h"
#include "strip.h"
#include "river-internal.h"

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                         FUNÇÕES DE CONFIGURAÇÃO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
void river_config_flux   (float flux)
{
    Config.flux = flux;
}

void river_config_island (float prob_island, int freq_island)
{
    Config.prob_island = prob_island;
    Config.freq_island = freq_island;
}

void river_config_size(int length, int height)
{
    Config.height = height;
    Config.length = length;
}

void river_config_margins(int zone)
{
    Config.zone = zone;
}

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                          FUNÇÕES DE ANIMAÇÃO
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
void river_animation_generate(int seed)
{
    /** VARIÁVEIS *****************************************************/
    int i = 0;                       /* Contador                     */
    TStrip new_line;                 /* Linha gerada                 */
    TStrip first_line;               /* 1ª linha gedara              */
    int zone = Config.zone;          /* 'Zona de conforto'           */
    float flux = Config.flux;        /* Fluxo total do rio           */
    int height = Config.height;      /* Altura total do grid         */
    int length = Config.length;      /* Largura total do rio         */
    int freq = Config.freq_island;   /* Distancia entre as ilhas     */
    float prob = Config.prob_island; /* Probabilidade de haver ilha  */
    Link new_node;

    /* Inicializa semente geradora de faixas de terreno
     * e cria lista para colocá-las: nosso cenário */
    tstrip_seed(seed); tstrip_island(prob, freq);
    river = list_init(height);

    /** INICIALIZA RIO ************************************************/
    /* Primeira linha, que servirá de base para todo o rio */
    first_line = tstrip_generate(length, zone, flux, NO_BASE, NULL);
    new_node = list_new_node(first_line);

    /* Preenche 'altura' faixas de terreno na lista: */
    list_insert(river, new_node);

    for(i = 1, base = first_line; i < height; i++, base = new_line)
    {
        new_line = tstrip_generate(length, zone, flux, base, NULL);
        new_node = list_new_node(new_line);
        list_insert(river, new_node);
    }

    /** IMPRIME RIO ***************************************************/
    list_select(river, HEAD, strip_print);
}

void river_animation_iterate()
{
    /** VARIÁVEIS *****************************************************/
    TStrip top/*, bottom*/;
    float flux = Config.flux;       /* Fluxo do rio              */
    int length = Config.length;     /* Largura do grid           */
    int zone = Config.zone;         /* Zona de conforto          */
    Link new_node;

    /** AVANÇA FAIXA DE TERRENO ***************************************/

   /* Libera linha do topo do grid ('saindo da tela') */
    new_node = list_prev(list_head(river));
    list_remove(river, new_node);

    /* Cria linha da base do grid ('entrando na tela') */
    top = tstrip_generate(length, zone, flux, base, list_item(new_node));
    base = top;
    list_insert(river, new_node);



    /** IMPRIME RIO ***************************************************/
    list_select(river, HEAD, strip_print);

}

void strip_print(TStrip strip)
{
    int i = 0; /* Contador */
    for(i = 0; i < Config.length; i++)
        printf("%c", strip[i].t);
    printf("\n");
}

void river_animation_finish()
{
    list_free(river);
    river = NULL;
    base = NULL;
}
