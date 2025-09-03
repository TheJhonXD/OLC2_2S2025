#include <stdio.h>
#include <stdlib.h>
#include "entorno/environment.h" /* Env_SaveVariable */
#include "entorno/ast.h"
#include "helpers/helper.h"
#include "ast/sentencias/params.h"

static Symbol getSymbol(TipoExpresion t, int lin, int col)
{
  switch (t)
  {
  case T_INTEGER:
    return SymInt(lin, col, 0);
    break;
  case T_FLOAT:
    return SymFloat(lin, col, 0.0);
    break;
  case T_BOOLEAN:
    return SymBool(lin, col, 0);
    break;
  case T_STRING:
    return SymStr(lin, col, "");
    break;
  case T_CHAR:
    return SymChar(lin, col, '\0');
    break;
  default:
    return SymNull(lin, col);
  }
}

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  (void)ast;
  Param *p = (Param *)self;
  Environment *E = (Environment *)env;

  Symbol tipoSym = getSymbol(p->tipo, p->base.lin, p->base.col);

  /* guarda la variable */
  Env_SaveVariable(E, p->id, tipoSym);
  return SymNull(p->base.lin, p->base.col);
}

static void Destruir(NodoBase *self)
{
  Param *d = (Param *)self;
  free(d->id);
  free(d);
}

Param *NewParam(int lin, int col, char *id, TipoExpresion tipo)
{
  Param *p = (Param *)malloc(sizeof(Param));
  NodoBase_init(&p->base, "Parametro funcion", lin, col, Ejecutar, Destruir);
  p->id = strDuplicate(id);
  p->tipo = tipo;
  p->next = NULL;
  return p;
}