#ifndef H_GUI_DEFINED
#define H_GUI_DEFINED

#include <allegro5/allegro.h>
ALLEGRO_DISPLAY *window;
ALLEGRO_EVENT_QUEUE *fila_eventos;

void gui_init();
void gui_create_land(float x1, float y1);
void gui_create_water(float x1, float y1);
void gui_create_window(int length, int height);
void gui_create_boat(float x, float y);
int gui_close_window();
void gui_create_margin_land_water(int x1, int y1, int x2, int y2, int x3, int y3);

#endif
