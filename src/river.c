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

/* Bibliotecas gráficas */
#include "allegro.h"

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

static int frame_height;



int old_left_margin = -1, old_right_margin = -1;

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

    frame_height = 0; /* Zera altura do frame de impressão para o rio */

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
    al_clear_to_color(al_map_rgb(0, 0, 0));
    list_select(river, HEAD, strip_print);
    al_flip_display();
}

int river_animation_iterate()
{
    /** VARIÁVEIS *****************************************************/
    TStrip top/*, bottom*/;
    float flux = Config.flux;       /* Fluxo do rio              */
    int length = Config.length;     /* Largura do grid           */
    int zone = Config.zone;         /* Zona de conforto          */
    Link new_node;

    frame_height = 0; /* Zera altura do frame de impressão para o rio */

    /** AVANÇA FAIXA DE TERRENO ***************************************/
    /* Libera linha do topo do grid ('saindo da tela') */
    new_node = list_prev(list_head(river));
    list_remove(river, new_node);

    /* Cria linha da base do grid ('entrando na tela') */
    top = tstrip_generate(length, zone, flux, base, list_item(new_node));
    base = top;
    list_insert(river, new_node);

    /** IMPRIME RIO ***************************************************/
    /* Elipse preenchido: x1, y1, raio x, raio y, cor */

    al_clear_to_color(al_map_rgb(0, 0, 0));
    list_select(river, HEAD, strip_print);
    gui_create_boat(Config.length*2.5, frame_height);
    al_flip_display();
    if(gui_close_window())
        return 1;
    return 0;
}

void strip_print(TStrip strip)
{
    int i = 0, k = 0; /* Contador */
    int old_m_l = 0, old_m_r = 0;
    int control1 = 0, control2 = 0;
    for(i = 0; i < Config.length-1; i++)
        if(strip[i].t == WATER && strip[i+1].t == LAND)
            k++;

    for(i = 0; i < Config.length; i++)
    {
        /* Começo de rio -- sem um elemento anterior */
        if(old_left_margin== -1)
            gui_create_land(i*5, frame_height);
        else if(i==Config.length-1 && old_right_margin == -1 )
            teste(i*5, frame_height);
        /* Terra seguida de água */
        else if(i!= Config.length-1 && strip[i].t == LAND && strip[i+1].t == WATER && ++control1==1)
        {
                old_m_l = i;
                if(i > old_left_margin)
                    gui_create_margin_land_water(i*5, frame_height+5, (i+1)*5, frame_height+5, i*5, frame_height);
                else
                    gui_create_land(i*5, frame_height);
        }
        /* terra seguida de água - pega a água depois de terra */
        else if(i!=0 && strip[i-1].t == LAND && strip[i].t == WATER && control1 == 1)
        {
                if(i-1 < old_left_margin)
                    gui_create_margin_land_water(i*5, frame_height, (i+1)*5, frame_height, i*5, frame_height+5);
                else
                    gui_create_water(i*5, frame_height);
        }
        /* água seguida de terra */
        else if(i!=0 && strip[i].t == LAND && strip[i-1].t == WATER && ((control1 == 1 && control2==1 && k<=1) || (control1==2 && control2 ==2)))
        {
                old_m_r = i;
                if(i < old_right_margin)
                    gui_create_margin_land_water((i+1)*5, frame_height+5, i*5, frame_height+5, (i+1)*5, frame_height);
                    /*gui_create_margin_land_water((i+1)*5, frame_height, i*5, frame_height, (i+1)*5, frame_height+5);*/
                else
                    gui_create_land(i*5, frame_height);
        }
        /* água seguida de terra - analisa água antes de terra */
        else if(i!= Config.length-1 && strip[i].t == WATER && strip[i+1].t == LAND && (( ++control2==1 && control1 == 1 && k<=1) || (control1==2 && control2 ==2)))
        {
                if(i+1 > old_right_margin)
                    gui_create_margin_land_water((i+1)*5, frame_height, i*5, frame_height, (i+1)*5, frame_height+5);
                    /*gui_create_margin_land_water((i+1)*5, frame_height+5, i*5, frame_height+5, (i+1)*5, frame_height);*/
                else
                    gui_create_water(i*5, frame_height);
        }
        else if(strip[i].t == LAND)
            gui_create_land(i * 5, frame_height);
        else if(strip[i].t == WATER)
            gui_create_water(i * 5, frame_height);

    }
    al_rest(1.4e-5);
    old_left_margin = old_m_l;
    old_right_margin = old_m_r;

    frame_height += 5;
}

void river_animation_finish()
{
    list_free(river);
    river = NULL;
    base = NULL;
}
