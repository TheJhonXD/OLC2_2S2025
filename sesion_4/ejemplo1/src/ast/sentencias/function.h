#ifndef FUNCTION_H
#define FUNCTION_H

#include "ast/AbstractTerminal.h"
#include "ast/sentencias/params.h"

typedef struct
{
  NodoBase base;
  char *id;           // Nombre de la funcion
  TipoExpresion tipo; // Tipo de la funcion
  Param *params;      // Parámetros de la funcion
  NodoBase *body;     // Cuerpo de la funcion
} Function;

Function *NewFunction(int lin, int col, char *id, TipoExpresion tipo, Param *params, NodoBase *body);

#endif // FUNCTION_H