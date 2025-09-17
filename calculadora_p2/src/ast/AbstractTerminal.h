#ifndef ABSTRACT_TERMINAL_H
#define ABSTRACT_TERMINAL_H

#include "entorno/symbol.h"
#include "entorno/ast.h"

typedef struct NodoBase NodoBase;

/* Ejecutar(ast, env) -> Symbol */
typedef Symbol (*MetodoEjecutar)(NodoBase *self, AST *ast, void *env);
typedef void (*MetodoDestruir)(NodoBase *self);

struct NodoBase
{
    char nombre[24];
    int lin, col;
    MetodoEjecutar ejecutar;
    MetodoDestruir destruir;
};

void NodoBase_init(NodoBase *n, const char *nombre, int lin, int col,
                   MetodoEjecutar ejecutar, MetodoDestruir destruir);

static inline Symbol NodoBase_Ejecutar(NodoBase *n, AST *ast, void *env)
{
    return (n && n->ejecutar) ? n->ejecutar(n, ast, env) : SymNull(0, 0);
}
static inline void NodoBase_Destruir(NodoBase *n)
{
    if (n && n->destruir)
        n->destruir(n);
}

#endif
