#ifndef BLOCK_STMT_H
#define BLOCK_STMT_H

#include "ast/AbstractTerminal.h"

typedef struct StmtCell
{
  struct NodoBase *stmt;
  struct StmtCell *next;
} StmtCell;

typedef struct
{
  NodoBase base;
  StmtCell *head;
  StmtCell *tail;
} Block;

Block *NewBlock(int lin, int col);        // Crea bloque vacío
void addBlockStmt(Block *b, NodoBase *s); // Agrega stmt al final

#endif
