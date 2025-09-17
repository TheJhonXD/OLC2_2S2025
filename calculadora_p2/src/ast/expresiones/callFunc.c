#include <stdio.h>
#include <stdlib.h>
#include "ast/expresiones/callFunc.h"
#include "ast/sentencias/params.h"
#include "entorno/FunctionSymbol.h"
#include "entorno/environment.h" /* Env_SaveVariable */
#include "entorno/ast.h"
#include "helpers/helper.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  CallFunc *call = (CallFunc *)self;
  Environment *caller = (Environment *)env;
  Symbol result = SymNull(self->lin, self->col);

  FunctionSymbol fsym = Env_GetFunction(caller, call->id);
  if (!fsym.id)
    return result; // No existe la función

  // Nuevo entorno
  Environment local;
  Env_init(&local, caller, call->id);

  // Enlazar parametros con argumentos
  Param *pp = fsym.ListDec;
  ArgList *aa = call->args;

  while (pp && aa)
  {
    Symbol v = NodoBase_Ejecutar(aa->expr, ast, caller); /* eval en caller */
    if (v.tipo != pp->tipo)
    {
      printf("Tipo incorrecto en parámetro '%s' de %s.\n", pp->id, call->id);
      char msg[256];
      snprintf(msg, sizeof msg, "Tipo incorrecto en parámetro '%s' de %s.\n", pp->id, call->id);
      AST_SetError(ast, msg);
      Env_free(&local);
      return result;
    }
    Env_SaveVariable(&local, pp->id, v);
    pp = pp->next;
    aa = aa->next;
  }

  if (pp || aa)
  {
    printf("Cantidad de parámetros no coincide en la llamada a %s.\n", call->id);
    char msg[256];
    snprintf(msg, sizeof msg,
             "Cantidad de parámetros no coincide en la llamada a %s.",
             call->id);
    AST_SetError(ast, msg);
    Env_free(&local);
    return result;
  }

  if (fsym.Block)
    (void)NodoBase_Ejecutar(fsym.Block, ast, &local);

  Env_free(&local);
  return result;
}

static void Destruir(NodoBase *self)
{
  CallFunc *c = (CallFunc *)self;
  free(c->id);
  free(c);
}

CallFunc *NewCallFunc(int lin, int col, char *id, ArgList *args)
{
  CallFunc *call = (CallFunc *)malloc(sizeof(CallFunc));
  NodoBase_init(&call->base, "CallFunc", lin, col, Ejecutar, Destruir);
  call->id = strDuplicate(id);
  call->args = args;
  return call;
}