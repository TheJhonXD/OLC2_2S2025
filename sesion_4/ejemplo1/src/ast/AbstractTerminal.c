#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast/AbstractTerminal.h"

void NodoBase_init(NodoBase *n, const char *nombre, int lin, int col,
                   MetodoEjecutar ejecutar, MetodoDestruir destruir)
{
    snprintf(n->nombre, sizeof(n->nombre), "%s", nombre ? nombre : "");
    n->lin = lin;
    n->col = col;
    n->ejecutar = ejecutar;
    n->destruir = destruir;
}
