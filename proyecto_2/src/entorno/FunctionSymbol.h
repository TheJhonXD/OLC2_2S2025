#ifndef FUNCTION_SYMBOL_H
#define FUNCTION_SYMBOL_H

#include "entorno/tipos.h"
#include "helpers/helper.h"

struct NodoBase; /* bloque/cuerpo */
struct Param;    /* lista enlazada de parámetros */

typedef struct
{
  int lin;
  int col;
  char *id;
  struct Param *ListDec;
  struct NodoBase *Block;
  TipoExpresion TipoRetorno;
} FunctionSymbol;

static inline FunctionSymbol FuncSym(int lin, int col, const char *id, struct Param *list_dec, TipoExpresion tipo_ret, struct NodoBase *block)
{
  FunctionSymbol f = {lin, col, (char *)id, list_dec, block, tipo_ret};
  return f;
}

/* Helper para construir un FunctionSymbol “nulo” */
static inline FunctionSymbol FuncSymNull(int lin, int col)
{
  FunctionSymbol f = {lin, col, NULL, NULL, NULL, T_NULL};
  return f;
}

#endif
