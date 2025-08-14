#include <stdio.h>
#include <string.h>
#include "ast.h"

void AST_Init(AST *a)
{
  if (!a)
    return;
  a->prints[0] = 0;
  a->errors[0] = 0;
}

void AST_SetError(AST *a, const char *msg)
{
  if (!a || !msg)
    return;
  size_t n = strlen(a->errors);
  snprintf(a->errors + n, sizeof(a->errors) - n, "%s\n", msg);
}

void AST_AppendPrint(AST *a, const char *txt)
{
  if (!a || !txt)
    return;
  size_t n = strlen(a->prints);
  snprintf(a->prints + n, sizeof(a->prints) - n, "%s", txt);
}
