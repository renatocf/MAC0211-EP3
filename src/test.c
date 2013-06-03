#include <stdio.h>
#include <stdlib.h>
#include "test.h"

/* Bibliotecas incluídas para o teste */
#include "list.h"
#include "strip.h"
#include "terrain.h"
#include "river-internal.h"

#define SIMPLE 1

static int maxl;
static int maxr;
static int maxl_mean = 0;
static int maxr_mean = 0;
static int test_mode = 0;
static int n_lines = 0;
static char last_char;

static void analyse_lines (TStrip strip);
static void print_lines (TStrip strip);
static void analyse_river (int seed);

void analyse_program(int seed, int iterations, int mode)
{
    int i = 0; /* Contador para número de iterações */
    test_mode = mode;
    
    for(i = 0; i < iterations-1; i++)
        { system("clear||cls"); river_animation_iterate(); }
    printf("\n"); analyse_river(seed);
}

void analyse_river(int seed)
{
    printf("\n\n\n\n\n");
    printf("Análise da estrutura do Jogo:\n");
    printf("-------------------------------------------------------\n");
    printf("\n");
    printf("Último frame:\n");
    printf("\n");
    list_select(river, HEAD, print_lines);
    printf("\n");
    printf("Configurações do rio\n");
    printf("\n");
    printf("* Fluxo: %.3f\n", Config.flux);
    printf("* Altura da tela: %d\n", Config.height);
    printf("* Largura da tela: %d\n", Config.length);
    printf("* 'Zona de conforto': %d\n", Config.zone);
    printf("* Probabilidade de gerar ilha: %.3f\n", Config.prob_island);
    printf("* Distância mínima para geração de ilhas: %d\n", Config.freq_island);
    printf("\n");

    n_lines = 0;
    list_select(river, HEAD, analyse_lines);

    printf("\n\n");
    printf("Média do limite da margem esquerda: %.2f\n",
            (float) maxl_mean/(Config.height));
    printf("Média do limite da margem direita:  %.2f\n",
            (float) maxr_mean/(Config.height));
}

static void print_lines(TStrip strip)
{
    int i = 0;      /* Contador */
    for(i = 0; i < Config.length; i++)
        printf("%c", strip[i].t);
    printf("\n");
}

static void analyse_lines(TStrip strip)
{
    int i = 0;      /* Contador */
    float flux = 0; /* Velocidade média */

    int n_water = 0; /* Pixels com água */
    int n_lands = 0; /* Pixels com terra */

    int lmargin = 0; /* Por causa das ilhas,
                        precisamos garantir que
                        a primeira margem esquerda
                        econtrada seja contabilizada */

    n_lines++; /* Total de linhas analisadas */
    last_char = strip[0].t; /* Primeiro terreno */

    /* Analisa/imprime a n_lines-ésima linha */
    printf("\nLinha %d:\n", n_lines);
    for(i = 0; i < Config.length; i++)
    {
        printf("%c", strip[i].t);
        if(strip[i].t == WATER)
        {
            if(last_char == LAND && !lmargin)
                { maxl = i; maxl_mean += i; lmargin = 1; }
            n_water++;
        }
        else
        {
            if(last_char == WATER)
                { maxr = i; maxr_mean += i; }
            n_lands++;
        }
        last_char = strip[i].t;
        flux += strip[i].v;
    }
    printf("\n");

    /* Relatório sobre a linha: */
    printf("Quantidade de água  (%c): %d\n", WATER, n_water);
    printf("Quantidade de terra (%c): %d\n", LAND, n_lands);
    printf("Posição da margem esquerda: %d\n", maxl);
    printf("Posição da margem direita:  %d\n", maxr);
    printf("Fluxo: %.3f\n", flux);
}
