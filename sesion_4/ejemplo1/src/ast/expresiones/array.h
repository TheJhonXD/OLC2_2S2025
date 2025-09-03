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

// suma impares hasta que i supera n
i = 0; s = 0;
do {
    if (i % 2 == 0) continue;  // salta al test sin sumar
    s = s + i;
    i = i + 1;
} while (i <= n);