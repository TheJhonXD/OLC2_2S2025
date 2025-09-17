#ifndef PARAMS_H
#define PARAMS_H

#include "ast/AbstractTerminal.h"
#include "entorno/tipos.h"

typedef struct Param
{
  NodoBase base;
  char *id;
  TipoExpresion tipo;
  struct Param *next;
} Param;

Param *NewParam(int lin, int col, char *id, TipoExpresion tipo);

#endif