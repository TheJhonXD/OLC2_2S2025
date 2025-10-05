#ifndef CALL_FUNC_H
#define CALL_FUNC_H

#include "ast/AbstractTerminal.h"
#include "ast/expresiones/args.h"

typedef struct
{
  NodoBase base;
  char *id;
  ArgList *args;
} CallFunc;

CallFunc *NewCallFunc(int lin, int col, char *id, ArgList *args);

#endif