/* Bibliotecas externas */
#include <stdio.h>
#include <stdlib.h>

/* Bibliotecas gráficas */

#include <allegro5/allegro_primitives.h>

/* Bibliotecas internas */
#include "allegro.h"

void gui_create_window(int length, int height)
{
    /* Variável representando a janela principal */
    window = NULL;

    /* Inicializamos a biblioteca
    al_init(); -->inicializado antes */

    /* Criamos a nossa janela - dimensões de largura x altura px */
    window = al_create_display(length, height);

    /* Preenchemos a janela de branco */
    al_clear_to_color(al_map_rgb(255, 255, 255));

    /* Atualiza a tela */
    al_flip_display();

    /* Segura a execução por 10 segundos -->tempo infinito
    al_rest(10.0); */

    /* Finaliza a janela
    al_destroy_display(janela); */
}

void gui_create_land(float x1, float y1)
{
    al_draw_filled_rectangle(x1, y1, x1+5, y1+5, al_map_rgb(139, 69, 19));
}

void gui_create_water(float x1, float y1)
{
    al_draw_filled_circle(x1+2.5, y1+2.5, 4.5, al_map_rgb(100, 149, 237));
    /* al_draw_filled_rectangle(x1, y1, x1+5, y1+5, al_map_rgb(100, 149, 237));*/
}

void gui_create_margin_left(float x1, float y1)
{
    al_draw_filled_circle(x1+2.5, y1+2.5, 4.5, al_map_rgb(139, 69, 19));
}

void gui_create_margin_right(float x1, float y1)
{
    al_draw_filled_circle(x1+2.5, y1+2.5, 4.5, al_map_rgb(139, 69, 19));
}

void gui_init()
{
    /* Inicializa a biblioteca allegro */
    al_init();
    al_init_primitives_addon();
}
