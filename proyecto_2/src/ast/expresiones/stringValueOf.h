#ifndef STRING_VALUEOF_H
#define STRING_VALUEOF_H

#include "ast/AbstractTerminal.h"

typedef struct StringValueOf
{
  NodoBase base;
  NodoBase *expr; // Expresión a convertir a string
} StringValueOf;

StringValueOf *NewStringValueOf(int lin, int col, NodoBase *expr);

#endif
