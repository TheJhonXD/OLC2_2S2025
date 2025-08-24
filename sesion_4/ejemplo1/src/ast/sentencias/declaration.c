#include <stdlib.h>
#include <stdio.h>
#include "ast/sentencias/declaration.h"
#include "entorno/environment.h" /* Env_SaveVariable */
#include "entorno/ast.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  (void)ast;
  Declaration *d = (Declaration *)self;
  Environment *E = (Environment *)env;

  Symbol v = NodoBase_Ejecutar(d->expr, ast, env);

  /* validación de tipo si se declaró explícito */
  if (v.tipo != d->tipo)
  {
    printf("Los tipos de datos son incorrectos\n");
    return SymNull(self->lin, self->col);
  }

  /* guarda la variable */
  Env_SaveVariable(E, d->id, v);
  return v;
}

static void Destruir(NodoBase *self)
{
  Declaration *d = (Declaration *)self;
  free(d->id);
  free(d);
}

Declaration *NewDeclaration(int lin, int col, char *id_aux, TipoExpresion tipo, NodoBase *expr)
{
  Declaration *d = (Declaration *)malloc(sizeof(Declaration));
  NodoBase_init(&d->base, "Declaration", lin, col, Ejecutar, Destruir);
  d->id = id_aux;
  d->tipo = tipo;
  d->expr = expr;
  return d;
}
