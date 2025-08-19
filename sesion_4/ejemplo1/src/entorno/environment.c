#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "environment.h"
/* Implementar en el futuro si sigo  */

Symbol Sym_copy(Symbol s)
{
  if (s.tipo == T_STRING && s.val.s)
  {
    size_t n = strlen(s.val.s) + 1;
    char *dup = (char *)malloc(n);
    if (dup)
      memcpy(dup, s.val.s, n);
    s.val.s = dup;
  }
  return s;
}
void Sym_dispose(Symbol *s)
{
  if (!s)
    return;
  if (s->tipo == T_STRING && s->val.s)
  {
    free((void *)s->val.s);
    s->val.s = NULL;
  }
}

/* ===== Entorno ===== */
static char *sdup(const char *s)
{
  size_t n = strlen(s) + 1;
  char *p = malloc(n);
  if (p)
    memcpy(p, s, n);
  return p;
}

static void ensure(Environment *e)
{
  if (e->Len == e->Cap)
  {
    e->Cap = e->Cap ? e->Cap * 2 : 16;
    e->Tabla = (VarEntry *)realloc(e->Tabla, e->Cap * sizeof(VarEntry));
  }
}

static int find_in_env(Environment *e, const char *id)
{
  for (int i = 0; i < e->Len; ++i)
    if (strcmp(e->Tabla[i].id, id) == 0)
      return i;
  return -1;
}

void Env_init(Environment *env, Environment *ant, const char *id)
{
  env->Anterior = ant;
  env->Tabla = NULL;
  env->Len = 0;
  env->Cap = 0;
  env->id = id ? id : "ENV";
}

void Env_free(Environment *env)
{
  if (!env)
    return;
  for (int i = 0; i < env->Len; ++i)
  {
    free(env->Tabla[i].id);
    Sym_dispose(&env->Tabla[i].val);
  }
  free(env->Tabla);
  env->Tabla = NULL;
  env->Len = env->Cap = 0;
}

void Env_SaveVariable(Environment *env, const char *id, Symbol value)
{
  int i = find_in_env(env, id);
  if (i != -1)
  {
    printf("La variable %s ya existe\n", id);
    return;
  }
  ensure(env);
  env->Tabla[env->Len].id = sdup(id);
  env->Tabla[env->Len].val = Sym_copy(value);
  env->Len++;
}

Symbol Env_GetVariable(Environment *env, const char *id)
{
  for (Environment *current = env; current; current = current->Anterior)
  {
    int i = find_in_env(current, id);
    if (i != -1)
    {
      return Sym_copy(current->Tabla[i].val);
    }
  }
  printf("La variable %s no existe\n", id);
  return SymNull(0, 0);
}

Symbol Env_SetVariable(Environment *env, const char *id, Symbol value)
{
  for (Environment *current = env; current; current = current->Anterior)
  {
    int i = find_in_env(current, id);
    if (i != -1)
    {
      if (current->Tabla[i].val.tipo == value.tipo)
      {
        Symbol prev = Sym_copy(current->Tabla[i].val);
        Sym_dispose(&current->Tabla[i].val);
        current->Tabla[i].val = Sym_copy(value);
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
