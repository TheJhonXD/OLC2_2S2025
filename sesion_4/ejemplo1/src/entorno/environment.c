#include "entorno/environment.h"
#include "helpers/helper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Busca en este entorno solamente
static VarNode *findHere(Environment *e, const char *id)
{
  for (VarNode *n = e->Tabla; n; n = n->next)
    if (strcmp(n->id, id) == 0)
      return n;
  return NULL;
}

static void free_string(Symbol *s)
{
  if (!s)
    return;

  if (s->tipo == T_STRING && s->val.s)
  {
    free(s->val.s);
    s->val.s = NULL;
  }
}

void Env_init(Environment *env, Environment *ant, const char *id)
{
  env->Anterior = ant;
  env->Tabla = NULL;
  env->id = id ? id : "ENV";
}

void Env_free(Environment *env)
{
  if (!env)
    return;
  VarNode *n = env->Tabla;
  while (n)
  {
    VarNode *next = n->next;
    free(n->id);
    free_string(&n->val);
    free(n);
    n = next;
  }
  env->Tabla = NULL;
}

// Crea la variable en el entorno actual
void Env_SaveVariable(Environment *env, const char *id, Symbol value)
{
  if (findHere(env, id))
  {
    printf("La variable %s ya existe\n", id);
    return;
  }
  VarNode *node = (VarNode *)malloc(sizeof(VarNode));
  node->id = strDuplicate(id);
  node->val = value;
  node->next = env->Tabla;
  env->Tabla = node;
}

// Busca en este entorno y sus padres
Symbol Env_GetVariable(Environment *env, const char *id)
{
  for (Environment *cur = env; cur; cur = cur->Anterior)
  {
    VarNode *n = findHere(cur, id);
    if (n)
      return n->val;
  }
  printf("La variable %s no existe\n", id);
  return SymNull(0, 0);
}

Symbol Env_SetVariable(Environment *env, const char *id, Symbol value)
{
  for (Environment *cur = env; cur; cur = cur->Anterior)
  {
    VarNode *n = findHere(cur, id);
    if (n)
    {
      if (n->val.tipo == value.tipo)
      {
        Symbol prev = n->val;
        /* libera string previo si lo había */
        free_string(&n->val);
        n->val = value; /* asigna el nuevo (shallow) */
        return prev;
      }
      else
      {
        printf("El tipo de dato es incorrecto\n");
        return SymNull(0, 0);
      }
    }
  }
  printf("La variable %s no existe\n", id);
  return SymNull(0, 0);
}
