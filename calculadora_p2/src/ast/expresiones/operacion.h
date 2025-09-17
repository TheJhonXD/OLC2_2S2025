#ifndef OPERACION_H
#define OPERACION_H

#include "ast/AbstractTerminal.h"

typedef struct
{
    NodoBase base;
    NodoBase *izq;
    const char *op; /* "+", "-", "*", "/", "<", ">", "==", "!=", "&&", "||" */
    NodoBase *der;
} Operation;

Operation *NewOperation(int lin, int col, NodoBase *izq, const char *op, NodoBase *der);

#endif
