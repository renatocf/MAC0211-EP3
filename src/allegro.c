#include <allegro.h>


void createWindow(int length, int height)
{
    /* Variável representando a janela principal */
    window = NULL;

    /* Inicializamos a biblioteca
    al_init(); -->inicializado antes */

    /* Criamos a nossa janela - dimensões de largura x altura px */
    janela = al_create_display(largura, altura);

    /* Preenchemos a janela de branco */
    al_clear_to_color(al_map_rgb(255, 255, 255));

    /* Atualiza a tela */
    al_flip_display();

    /* Segura a execução por 10 segundos -->tempo infinito
    al_rest(10.0); */

    /* Finaliza a janela
    al_destroy_display(janela); */
}

void allegroInit()
{
    /* Inicializa a biblioteca allegro */
    al_init();
}
