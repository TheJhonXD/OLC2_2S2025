#ifndef DECL_H
#define DECL_H

#include "ast/AbstractTerminal.h"
#include "entorno/tipos.h"

typedef struct
{
  NodoBase base;
  char *id;
  TipoExpresion tipo;
  NodoBase *expr;
} Declaration;

Declaration *NewDeclaration(int lin, int col, char *id_aux, TipoExpresion tipo, NodoBase *expr);

#endif
