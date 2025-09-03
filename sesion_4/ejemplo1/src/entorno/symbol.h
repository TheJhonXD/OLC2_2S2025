#ifndef SYMBOL_H
#define SYMBOL_H

#include "entorno/tipos.h"
#include "helpers/helper.h"

typedef struct
{
  int tam;
  struct Symbol_ *items;
} ArrayType;

typedef struct Symbol_
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
    char c;
    ArrayType *arr;
  } val;
} Symbol;

static inline Symbol SymInt(int lin, int col, int v)
{
  Symbol s = {lin, col, T_INTEGER, {0}};
  s.val.i = v;
  return s;
}
static inline Symbol SymFloat(int lin, int col, double v)
{
  Symbol s = {lin, col, T_FLOAT, {0}};
  s.val.f = v;
  return s;
}
static inline Symbol SymBool(int lin, int col, int v)
{
  Symbol s = {lin, col, T_BOOLEAN, {0}};
  s.val.b = !!v;
  return s;
}
static inline Symbol SymStr(int lin, int col, const char *v)
{
  Symbol s = {lin, col, T_STRING, {0}};
  s.val.s = v ? strDuplicate(v) : NULL;
  return s;
}
static inline Symbol SymChar(int lin, int col, char v)
{
  Symbol s = {lin, col, T_CHAR, {0}};
  s.val.c = v;
  return s;
}
static inline Symbol SymNull(int lin, int col)
{
  Symbol s = {lin, col, T_NULL, {0}};
  return s;
}

#endif
