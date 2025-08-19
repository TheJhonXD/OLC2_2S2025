#include <stdlib.h>
#include <stdio.h>
#include "ast/sentencias/assigment.h"
#include "entorno/environment.h" /* Env_SaveVariable */
#include "entorno/ast.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  (void)ast;
  Assigment *d = (Assigment *)self;
  Environment *E = (Environment *)env;

  Symbol v = NodoBase_Ejecutar(d->expr, ast, env);

  /* guarda variable */
  Env_SetVariable(E, d->id, v);
  return v;
}

static void Destruir(NodoBase *self)
{
  Assigment *d = (Assigment *)self;
  free(d->id);
  free(d);
}

Assigment *NewAssigment(int lin, int col, char *id_aux, NodoBase *expr)
{
  Assigment *d = (Assigment *)malloc(sizeof(Assigment));
  NodoBase_init(&d->base, "Assigment", lin, col, Ejecutar, Destruir);
  d->id = id_aux;
  d->expr = expr;
  return d;
}
