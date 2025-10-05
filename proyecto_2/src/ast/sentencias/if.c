#include <stdlib.h>
#include <stdio.h>
#include "ast/sentencias/if.h"
#include "entorno/environment.h" /* Env_SaveVariable */
#include "entorno/ast.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  (void)ast;
  If *d = (If *)self;
  Environment *E = (Environment *)env;

  Symbol v = NodoBase_Ejecutar(d->condicion, ast, env);

  /* validación de tipo */
  if (v.tipo != T_BOOLEAN)
  {
    printf("El tipo de expresión es incorrecto para un If\n");
    return SymNull(self->lin, self->col);
  }

  if (v.val.b == 1)
  {
    Environment if_stmt;
    Env_init(&if_stmt, NULL, "IF_STMT");
    (void)NodoBase_Ejecutar(d->bloque, ast, &if_stmt);
    Env_free(&if_stmt);
  }

  return SymNull(self->lin, self->col);
}

static void Destruir(NodoBase *self)
{
  If *d = (If *)self;
  free(d);
}

If *NewIf(int lin, int col, NodoBase *condicion, NodoBase *bloque)
{
  If *d = (If *)malloc(sizeof(If));
  NodoBase_init(&d->base, "IF", lin, col, Ejecutar, Destruir);
  d->condicion = condicion;
  d->bloque = bloque;
  return d;
}
