#include <stdlib.h>
#include <stdio.h>
#include "ast/expresiones/stringValueOf.h"
#include "entorno/ast.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  StringValueOf *sv = (StringValueOf *)self;

  // Evaluar la expresión
  Symbol val = NodoBase_Ejecutar(sv->expr, ast, env);

  // Convertir a string según el tipo
  char buffer[256];

  switch (val.tipo)
  {
  case T_INTEGER:
    snprintf(buffer, sizeof(buffer), "%d", val.val.i);
    break;
  case T_FLOAT:
    snprintf(buffer, sizeof(buffer), "%f", val.val.f);
    break;
  case T_BOOLEAN:
    snprintf(buffer, sizeof(buffer), "%s", val.val.b ? "true" : "false");
    break;
  case T_CHAR:
    snprintf(buffer, sizeof(buffer), "%c", val.val.c);
    break;
  case T_STRING:
    // Ya es string, retornar tal cual
    return val;
  default:
    snprintf(buffer, sizeof(buffer), "null");
    break;
  }

  // Crear y retornar el símbolo de string
  return SymStr(self->lin, self->col, strdup(buffer));
}

static void Destruir(NodoBase *self)
{
  StringValueOf *sv = (StringValueOf *)self;
  if (sv->expr)
  {
    NodoBase_Destruir(sv->expr);
  }
  free(sv);
}

StringValueOf *NewStringValueOf(int lin, int col, NodoBase *expr)
{
  StringValueOf *sv = (StringValueOf *)malloc(sizeof(StringValueOf));
  NodoBase_init(&sv->base, "StringValueOf", lin, col, Ejecutar, Destruir);
  sv->expr = expr;
  return sv;
}
