/* Bibliotecas externas */
#include <stdio.h>
#include <stdlib.h>

/* Bibliotecas gráficas */

#include <allegro5/allegro_primitives.h>

/* Bibliotecas internas */
#include "allegro.h"

void gui_create_window(int length, int height)
{
    fila_eventos = NULL;
    fila_eventos = al_create_event_queue();
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
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(window);
    }

    al_set_window_title(window, "Jogo da canoa");
    al_register_event_source(fila_eventos, al_get_display_event_source(window));

}

void gui_create_land(float x1, float y1)
{
    al_draw_filled_rectangle(x1, y1, x1+5, y1+5, al_map_rgb(139, 69, 19));
}

void gui_create_water(float x1, float y1)
{
    al_draw_filled_rectangle(x1, y1, x1+5, y1+5, al_map_rgb(100, 149, 237));
}


void gui_init()
{
    /* Inicializa a biblioteca allegro */
    al_init();
    al_init_primitives_addon();
}

void gui_create_boat(float x, float y)
{
    al_draw_filled_ellipse(x, y -20.0 ,10.0 , 20.0, al_map_rgb(139, 87, 66));
}

int gui_close_window()
{
    if (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                al_destroy_event_queue(fila_eventos);
                al_destroy_display(window);
                return 1;
            }
        }
        return 0;
}

void gui_create_margin_land_water(int x1, int y1, int x2, int y2, int x3, int y3)
{
    /* Triângulo preenchido: x1, y1, x2, y2, x3, y3, cor*/
    al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, al_map_rgb(139, 69, 19));
    al_draw_filled_triangle(x2, y2, x3, y3, x2, y3, al_map_rgb(100, 149, 237));

    /*al_draw_filled_triangle(x1, y1, x2, y2, x3, y3, al_map_rgb(255, 20, 147));
    al_draw_filled_triangle(x2, y2, x3, y3, x2, y3, al_map_rgb(255, 255, 0));*/
}
