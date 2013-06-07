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

void gui_create_boat(float x, float y)
{
    al_draw_filled_ellipse(x, y -20.0 ,10.0 , 20.0, al_map_rgb(0,205,102));
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

/*bool inicializar()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return false;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }

    al_set_window_title(janela, "Animando!!!");

    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }

    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    return true;
}*/
