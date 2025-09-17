#ifndef AST_H
#define AST_H

typedef struct AST {
    char prints[4096];
    char errors[4096];
} AST;

void AST_Init(AST* a);
void AST_SetError(AST* a, const char* msg);
void AST_AppendPrint(AST* a, const char* txt);

#endif
