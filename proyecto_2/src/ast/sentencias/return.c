#include <stdlib.h>
#include "ast/sentencias/return.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  (void)ast;
  (void)env;
  Return *r = (Return *)self;

  // Si hay expresión, evaluarla y retornar su valor
  if (r->expr)
  {
    return NodoBase_Ejecutar(r->expr, ast, env);
  }

  // Return sin valor
  return SymNull(self->lin, self->col);
}

static void Destruir(NodoBase *self)
{
  Return *r = (Return *)self;
  if (r->expr)
  {
    NodoBase_Destruir(r->expr);
  }
  free(r);
}

Return *NewReturn(int lin, int col, NodoBase *expr)
{
  Return *r = (Return *)malloc(sizeof(Return));
  NodoBase_init(&r->base, "Return", lin, col, Ejecutar, Destruir);
  r->expr = expr;
  return r;
}
