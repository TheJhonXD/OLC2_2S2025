#ifndef PRIMITIVOS_H
#define PRIMITIVOS_H

#include "ast/AbstractTerminal.h"

typedef struct
{
  NodoBase base;
  Symbol s; /* Lin/Col/Tipo/Valor */
} Primitive;

Primitive *NewPrimitive(int lin, int col, Symbol s);

#endif
