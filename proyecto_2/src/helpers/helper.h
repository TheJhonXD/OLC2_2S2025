#ifndef HELPER_H
#define HELPER_H

#include <string.h>
#include <stdlib.h>

// Para duplicar cadenas
static char *strDuplicate(const char *s)
{
  size_t n = strlen(s) + 1;
  char *p = (char *)malloc(n);
  if (p)
    memcpy(p, s, n);
  return p;
}

#endif
