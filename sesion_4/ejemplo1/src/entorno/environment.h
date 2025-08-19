#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "entorno/symbol.h"

typedef struct
{
    char *id;
    Symbol val;
} VarEntry;

typedef struct
{
    struct Environment *Anterior; // Entorno padre
    VarEntry *Tabla;
    int Len, Cap; // Usados y capacidad
    const char *id;

} Environment;

void Env_init(Environment *env, Environment *anterior, const char *id);
void Env_free(Environment *env);

void Env_SaveVariable(Environment *env, const char *id, Symbol value);
Symbol Env_GetVariable(Environment *env, const char *id);
Symbol Env_SetVariable(Environment *env, const char *id, Symbol value);

/* helpers para strings en Symbol */
Symbol Sym_copy(Symbol s);
void Sym_dispose(Symbol *s);

#endif
