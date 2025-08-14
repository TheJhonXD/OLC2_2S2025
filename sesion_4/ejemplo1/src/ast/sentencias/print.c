#include <stdio.h>
#include <stdlib.h>
#include "ast/sentencias/print.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
    Print *p = (Print *)self;
    Symbol s = NodoBase_Ejecutar(p->expr, ast, env);

    char buf[64];
    switch (s.tipo)
    {
    case T_INTEGER:
        snprintf(buf, sizeof buf, "%d\n", s.val.i);
        break;
    case T_FLOAT:
        snprintf(buf, sizeof buf, "%g\n", s.val.f);
        break;
    case T_BOOLEAN:
        snprintf(buf, sizeof buf, "%s\n", s.val.b ? "true" : "false");
        break;
    case T_STRING:
        snprintf(buf, sizeof buf, "%s\n", s.val.s ? s.val.s : "");
        break;
    default:
        snprintf(buf, sizeof buf, "null\n");
        break;
    }
    AST_AppendPrint(ast, buf);
    return s; /* o SymNull(self->lin,self->col) */
}

static void Destruir(NodoBase *self)
{
    Print *p = (Print *)self;
    NodoBase_Destruir(p->expr);
    free(self);
}

Print *NewPrint(int lin, int col, NodoBase *expr)
{
    Print *n = (Print *)malloc(sizeof(Print));
    NodoBase_init(&n->base, "Print", lin, col, Ejecutar, Destruir);
    n->expr = expr;
    return n;
}
