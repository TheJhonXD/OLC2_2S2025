#include <stdlib.h>
#include <stdio.h>
#include "ast/expresiones/variable.h"
#include "entorno/environment.h"
#include "entorno/ast.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  Variable *v = (Variable *)self;
  Environment *E = (Environment *)env;

  /* Buscar valor actual de la variable en el entorno */
  Symbol s = Env_GetVariable(E, v->id);

  /* Si no existe, reporta también al AST */
  if (s.tipo == T_NULL)
  {
    char msg[256];
    snprintf(msg, sizeof msg,
             "[%d:%d][%s] La variable '%s' no existe",
             self->lin, self->col, (E && E->id) ? E->id : "ENV", v->id);
    AST_SetError(ast, msg);
    return SymNull(self->lin, self->col);
  }

  return s;
}

static void Destruir(NodoBase *self)
{
  Variable *v = (Variable *)self;
  free(v->id);
  free(v);
}

Variable *NewVariable(int lin, int col, char *id_aux)
{
  Variable *v = (Variable *)malloc(sizeof(Variable));
  NodoBase_init(&v->base, "Variable", lin, col, Ejecutar, Destruir);
  v->id = id_aux;
  return v;
}
