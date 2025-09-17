#ifndef PRINT_STMT_H
#define PRINT_STMT_H

#include "ast/AbstractTerminal.h"

typedef struct
{
  NodoBase base;
  NodoBase *expr;
} Print;

Print *NewPrint(int lin, int col, NodoBase *expr);

#endif
