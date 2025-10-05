#ifndef ASSIGMENT_H
#define ASSIGMENT_H

#include "ast/AbstractTerminal.h"

typedef struct
{
  NodoBase base;
  char *id;
  NodoBase *expr;
} Assigment;

Assigment *NewAssigment(int lin, int col, char *id_aux, NodoBase *expr);

#endif
