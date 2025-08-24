#ifndef IF_H
#define IF_H

#include "ast/AbstractTerminal.h"

typedef struct If
{
  NodoBase base;
  NodoBase *condicion;
  NodoBase *bloque;
} If;

If *NewIf(int lin, int col, NodoBase *condicion, NodoBase *bloque);

#endif
