#include <stdlib.h>
#include "ast/expresiones/primitivos.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
    (void)ast;
    (void)env;
    return ((Primitive *)self)->s;
}
static void Destruir(NodoBase *self) { free(self); }

Primitive *NewPrimitive(int lin, int col, Symbol s)
{
    Primitive *n = (Primitive *)malloc(sizeof(Primitive));
    NodoBase_init(&n->base, "Primitive", lin, col, Ejecutar, Destruir);
    n->s = s;
    return n;
}
