#ifndef ARGS_H
#define ARGS_H

#include "ast/AbstractTerminal.h"

typedef struct ArgList
{
  NodoBase *expr;
  struct ArgList *next;
} ArgList;

static inline ArgList *NewArgList(NodoBase *expr, ArgList *next)
{
  ArgList *list = (ArgList *)malloc(sizeof(ArgList));
  list->expr = expr;
  list->next = next;
  return list;
}

#endif