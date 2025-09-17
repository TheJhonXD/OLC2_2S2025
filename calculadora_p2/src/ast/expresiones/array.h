#ifndef ARRAY_H
#define ARRAY_H

#include "entorno/symbol.h"
#include "ast/expresiones/args.h"

typedef struct
{
  NodoBase base;
  ArgList *elementos;
  int size;
} Array;

Array *NewArray(int lin, int col, ArgList *elementos)

#endif