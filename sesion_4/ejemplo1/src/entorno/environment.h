#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "entorno/symbol.h"
#include "entorno/FunctionSymbol.h"

// Lista enlazada: id → valor
typedef struct VarNode
{
    char *id;
    Symbol val;
    struct VarNode *next;
} VarNode;

typedef struct FuncNode
{
    char *id;
    FunctionSymbol fun;
    struct FuncNode *next;
} FuncNode;

typedef struct Environment
{
    struct Environment *Anterior; // Entorno padre
    VarNode *Tabla;
    const char *id; // Nombre del scope
    FuncNode *Functions;
} Environment;

void Env_init(Environment *env, Environment *ant, const char *id);
void Env_free(Environment *env);

void Env_SaveVariable(Environment *env, const char *id, Symbol value);
Symbol Env_GetVariable(Environment *env, const char *id);
Symbol Env_SetVariable(Environment *env, const char *id, Symbol value);

void Env_SaveFunction(Environment *env, const char *id, FunctionSymbol func);
FunctionSymbol Env_GetFunction(Environment *env, const char *id);

#endif
