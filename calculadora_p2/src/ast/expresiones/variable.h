#ifndef VARIABLE_H
#define VARIABLE_H

#include "ast/AbstractTerminal.h"

typedef struct {
  NodoBase base;
  char* id;
} Variable;

Variable* NewVariable(int lin, int col, char* id_aux);

#endif