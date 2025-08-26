#include <stdlib.h>
#include "ast/sentencias/block.h"

static Symbol Ejecutar(NodoBase *self, AST *ast, void *env)
{
  Block *b = (Block *)self;
  for (StmtCell *c = b->head; c; c = c->next)
  {
    (void)NodoBase_Ejecutar(c->stmt, ast, env);
  }
  return SymNull(self->lin, self->col);
}

static void Destruir(NodoBase *self)
{
  Block *b = (Block *)self;
  StmtCell *c = b->head;
  while (c)
  {
    StmtCell *nx = c->next;
    if (c->stmt)
      NodoBase_Destruir(c->stmt);
    free(c);
    c = nx;
  }
  free(b);
}

Block *NewBlock(int lin, int col)
{
  Block *b = (Block *)malloc(sizeof(Block));
  NodoBase_init(&b->base, "Block", lin, col, Ejecutar, Destruir);
  b->head = b->tail = NULL;
  return b;
}

void addBlockStmt(Block *b, NodoBase *s)
{
  if (!b || !s)
  {
    return;
  }
  StmtCell *c = (StmtCell *)malloc(sizeof(StmtCell));
  c->stmt = s;
  c->next = NULL;
  if (!b->head)
    b->head = b->tail = c;
  else
  {
    b->tail->next = c;
    b->tail = c;
  }
}
