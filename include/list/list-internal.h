#ifndef H_LIST_INTERNAL_DEFINED
#define H_LIST_INTERNAL_DEFINED

#include "list.h"

/* Estrutura para lista:
 * Ponteiro para a cabeça da lista */
struct list {
    Link head;
};

/* Estrutura para nódulo da lista:
 * Campo de estrutura + ponteiro para o próximo */
struct lnode {
    LItem item;
    Link  next;
    Link  prev;
};

#endif
