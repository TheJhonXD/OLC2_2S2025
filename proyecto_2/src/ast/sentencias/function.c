#include <stdlib.h>
#include <stdio.h>
#include "ast/sentencias/function.h"
#include "entorno/environment.h" /* Env_SaveVariable */
#include "entorno/ast.h"
#include "helpers/helper.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  (void)ast;
  Function *d = (Function *)self;
  Environment *E = (Environment *)env;

  FunctionSymbol fsym = FuncSym(d->base.lin, d->base.col, d->id, d->params, d->tipo, d->body);

  /* guarda la funcion */
  Env_SaveFunction(E, d->id, fsym);
  return SymNull(d->base.lin, d->base.col);
}

static void Destruir(NodoBase *self)
{
  Function *d = (Function *)self;
  free(d->id);
  free(d);
}

Function *NewFunction(int lin, int col, char *id, TipoExpresion tipo, Param *params, NodoBase *body)
{
  Function *f = (Function *)malloc(sizeof(Function));
  NodoBase_init(&f->base, "Function", lin, col, Ejecutar, Destruir);
  f->id = strDuplicate(id);
  f->tipo = tipo;
  f->params = params;
  f->body = body;
  return f;
}