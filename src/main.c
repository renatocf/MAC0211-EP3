/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                              BIBLIOTECAS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "test.h"
#include "utils.h"
#include "river.h"
#include "strip.h"
#include "getopt.h"

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                 MACROS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* Padrões para a gerar o rio */
#define FLUX       73.54
#define HEIGHT     30
#define LENGTH     100
#define ITERATIONS 10
#define ZONE       10
#define ISLAND     0.4
#define SEED       13
#define FREQ       5

/* Intervalo de tempo da animação */
#define INTERVAL   1.4*10e1

/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                          ESTRUTURAS/MENSAGENS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/

/* Opções da linha de comando */
typedef struct options
{
    /* Opções de configuração */
    float F; /* Fluxo */
    int H;   /* Altura do rio */
    int L;   /* Largura do rio */
    int N;   /* Número de iterações */
    int Z;   /* Distancia de segurança entre as margens */
    float i; /* Probabilidade de gerar ilha */
    int s;   /* Semente */
    int f;   /* Frequência com que as ilhas aparecem
                (em número de linhas) */

    /* Opções booleanas */
    int t;   /* Modo teste simples */
    int T;   /* Modo teste completo */
    int h;   /* Ajuda */
} Options;

/* Mensagens */
char help[] = "Jogo das canoas!\n"
              "\n"
              "Opções:\n"
              "-F fluxo de cada linha\n"
              "-H altura da janela\n"
              "-L largura da janela\n"
              "-Z distancia mínima entre as margens\n"
              "-i probabilidade de haver ilha em determinada linha\n"
              "-s seed para a geração de números pseudo-aleatórios\n"
              "-f distancia mínima entre as ilhas\n"
              "-h ajuda\n"

              "Para mais informações veja a documentação.\n";


/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                               PROTÓTIPOS
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
int receive_arguments (int argc, char **argv, Options *args);


/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                  MAIN
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
int main(int argc, char **argv)
{
    /** VARIÁVEIS *****************************************************/
    int func_err; int test_mode;
    clock_t init, end;

    /* Struct com argumentos da linha de comando */
    Options args = { FLUX, HEIGHT, LENGTH, ITERATIONS, ZONE, ISLAND,
                     SEED, FREQ, 0, 0, 0};

    /** ARGUMENTOS ****************************************************/
    func_err = receive_arguments(argc, argv, &args);
    if(func_err) return EXIT_FAILURE;

    if(args.h == 1)
    {
        printf("\n%s\n", help);
        return EXIT_SUCCESS;
    }

    /** CONFIGURAÇÕES DO RIO ******************************************/
    river_config_flux    (args.F);
    river_config_size    (args.L, args.H);
    river_config_island  (args.i, args.f);
    river_config_margins (args.Z);

    /** ANIMAÇÃO DO RIO ***********************************************/
    river_animation_generate(args.s);

    test_mode = args.t + args.T;
    if(test_mode) analyse_program(args.s, args.N, test_mode);
    else while(1)
    {
        for(end = init = clock(); end-init < INTERVAL; end = clock());
        system("clear || cls");
        river_animation_iterate();
    }

    /** LIBERAÇÃO DE MEMÓRIA ******************************************/
    river_animation_finish();

    return EXIT_SUCCESS;
}


/*
////////////////////////////////////////////////////////////////////////
-----------------------------------------------------------------------
                                 FUNÇÕES
-----------------------------------------------------------------------
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
*/
int receive_arguments(int argc, char **argv, Options *args)
/* Recebe os argumentos da linha de comando e os
 * armazena na struct correspondente */
{
    char opt;
    while((opt = getopt(argc, argv, "F:H:L:N:Z:i:s:f:tTh")) != NONE)
    {
        switch(opt)
        {
        case 'F':
            args->F = atof(optarg);
            break;
        case 'H':
            args->H = atoi(optarg);
            break;
        case 'L':
            args->L = atoi(optarg);
            break;
        case 'N':
            args->N = atoi(optarg);
            break;
        case 'Z':
            args->Z = atoi(optarg);
            break;
        case 'i':
            args->i = atof(optarg);
            break;
        case 's':
            args->s = atoi(optarg);
            break;
        case 'f':
            args->f = atoi(optarg);
            break;
        case 't':
            args->t = 1;
            break;
        case 'T':
            args->T = 2;
            break;
        case 'h':
            args->h = 1;
            break;
        case '?':
            printf("Argumento -%c desconhecido\n", optopt);
            return EXIT_FAILURE;
        }
    } /* while */
    return EXIT_SUCCESS;
}
