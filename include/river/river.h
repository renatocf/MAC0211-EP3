#ifndef H_RIVER_DEFINED
#define H_RIVER_DEFINED

/* Funções para configuração de parâmetros */
void river_config_flux        (float);
void river_config_size        (int, int);   /* largura/altura */
void river_config_island      (float, int); /* probabilidade/frequência da ilha */
void river_config_margins     (int);   /* zona de conforto */

/* Funções para controle de animação */
void river_animation_generate (int);        /* semente */
void river_animation_iterate  ();
void river_animation_finish   ();

#endif
