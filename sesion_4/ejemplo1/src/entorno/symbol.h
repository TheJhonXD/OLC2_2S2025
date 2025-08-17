#ifndef SYMBOL_H
#define SYMBOL_H

#include "entorno/tipos.h"

typedef struct
{
  int lin;
  int col;
  TipoExpresion tipo;
  union
  {
    int i;
    double f;
    int b;
    char *s;
  } val;
} Symbol;

static inline Symbol SymInt(int lin, int col, int v)
{
  Symbol s = {lin, col, T_INTEGER};
  s.val.i = v;
  return s;
}
static inline Symbol SymFloat(int lin, int col, double v)
{
  Symbol s = {lin, col, T_FLOAT};
  s.val.f = v;
  return s;
}
static inline Symbol SymBool(int lin, int col, int v)
{
  Symbol s = {lin, col, T_BOOLEAN};
  s.val.b = !!v;
  return s;
}
static inline Symbol SymStr(int lin, int col, const char *v)
{
  Symbol s = {lin, col, T_STRING};
  s.val.s = v;
  return s;
}
static inline Symbol SymNull(int lin, int col)
{
  Symbol s = {0};
  s.lin = lin;
  s.col = col;
  s.tipo = T_NULL;
  return s;
}

#endif
