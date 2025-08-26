#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast/expresiones/operacion.h"

static int isInt(Symbol s) { return s.tipo == T_INTEGER; }
static int isFlt(Symbol s) { return s.tipo == T_FLOAT; }
static double asFlt(Symbol s) { return isFlt(s) ? s.val.f : (double)s.val.i; }

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  Operation *o = (Operation *)self;
  Symbol a = NodoBase_Ejecutar(o->izq, ast, env);
  Symbol b = NodoBase_Ejecutar(o->der, ast, env);

  if (strcmp(o->op, "+") == 0)
  {
    if (isInt(a) && isInt(b))
      return SymInt(o->base.lin, o->base.col, a.val.i + b.val.i);
    return SymFloat(o->base.lin, o->base.col, asFlt(a) + asFlt(b));
  }
  if (strcmp(o->op, "-") == 0)
  {
    if (isInt(a) && isInt(b))
      return SymInt(o->base.lin, o->base.col, a.val.i - b.val.i);
    return SymFloat(o->base.lin, o->base.col, asFlt(a) - asFlt(b));
  }
  if (strcmp(o->op, "*") == 0)
  {
    if (isInt(a) && isInt(b))
      return SymInt(o->base.lin, o->base.col, a.val.i * b.val.i);
    return SymFloat(o->base.lin, o->base.col, asFlt(a) * asFlt(b));
  }
  if (strcmp(o->op, "/") == 0)
  {
    double y = asFlt(b);
    if (y == 0.0)
    {
      AST_SetError(ast, "ERROR: No es posible dividir en cero");
      return SymNull(o->base.lin, o->base.col);
    }
    return SymFloat(o->base.lin, o->base.col, asFlt(a) / y);
  }
  if (strcmp(o->op, "unario") == 0)
  {
    if (isInt(a))
      return SymInt(o->base.lin, o->base.col, -a.val.i);
    return SymFloat(o->base.lin, o->base.col, -asFlt(a));
  }

  if (strcmp(o->op, ">") == 0)
  {
    return SymBool(o->base.lin, o->base.col, a.val.i > b.val.i);
  }

  AST_SetError(ast, "ERROR: Operador no soportado");
  return SymNull(o->base.lin, o->base.col);
}

static void Destruir(NodoBase *self)
{
  Operation *o = (Operation *)self;
  NodoBase_Destruir(o->izq);
  NodoBase_Destruir(o->der);
  free(self);
}

Operation *NewOperation(int lin, int col, NodoBase *izq, const char *op, NodoBase *der)
{
  Operation *n = (Operation *)malloc(sizeof(Operation));
  NodoBase_init(&n->base, "Operation", lin, col, Ejecutar, Destruir);
  n->izq = izq;
  n->op = op;
  n->der = der;
  return n;
}
