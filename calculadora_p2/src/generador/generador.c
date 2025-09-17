#include "generador.h"
#include <string.h>

Generator g_gen;

void initGenerator()
{
  g_gen.temporal = 1; // Empieza desde t1
  g_gen.label = 1;    // Empieza desde L1
  g_gen.num_lines = 0;
  g_gen.num_temps = 0;
}

char *newTemp()
{
  static char temp[32];
  snprintf(temp, sizeof(temp), "t%d", g_gen.temporal);

  // Guardar en lista de temporales
  strcpy(g_gen.temps[g_gen.num_temps], temp);
  g_gen.num_temps++;

  g_gen.temporal++;
  return temp;
}

char *newLabel()
{
  static char label[32];
  snprintf(label, sizeof(label), "L%d", g_gen.label);
  g_gen.label++;
  return label;
}

void addExpression(const char *target, const char *left, const char *operator, const char *right)
{
  snprintf(g_gen.code[g_gen.num_lines], 256, "%s = %s %s %s;", target, left, operator, right);
  g_gen.num_lines++;
}

void addAssign(const char *target, const char *value)
{
  snprintf(g_gen.code[g_gen.num_lines], 256, "%s = %s;", target, value);
  g_gen.num_lines++;
}

void addLabel(const char *label)
{
  snprintf(g_gen.code[g_gen.num_lines], 256, "%s:", label);
  g_gen.num_lines++;
}

void addGoto(const char *label)
{
  snprintf(g_gen.code[g_gen.num_lines], 256, "goto %s;", label);
  g_gen.num_lines++;
}

void addIf(const char *left, const char *operator, const char *right, const char *label)
{
  snprintf(g_gen.code[g_gen.num_lines], 256, "if(%s %s %s) goto %s;", left, operator, right, label);
  g_gen.num_lines++;
}

void addPrint(const char *value)
{
  snprintf(g_gen.code[g_gen.num_lines], 256, "printf(\"%%d\", %s);", value);
  g_gen.num_lines++;
}

void addComment(const char *comment)
{
  snprintf(g_gen.code[g_gen.num_lines], 256, "// %s", comment);
  g_gen.num_lines++;
}

void printCode()
{
  printf("\n Solo codigo intermedio \n");
  for (int i = 0; i < g_gen.num_lines; i++)
  {
    printf("%d: %s\n", i, g_gen.code[i]);
  }
  printf("=========================\n\n");
}

void printFinalCode()
{
  printf("/* HEADERS */\n");
  printf("#include <stdio.h>\n");
  printf("#include <math.h>\n");
  printf("int ");

  // Declarar temporales
  if (g_gen.num_temps > 0)
  {
    printf("%s", g_gen.temps[0]);
    for (int i = 1; i < g_gen.num_temps; i++)
    {
      printf(", %s", g_gen.temps[i]);
    }
    printf(";\n\n");
  }

  printf("/*------MAIN------*/\n");
  printf("void main() {\n");

  // Imprimir código con indentación
  for (int i = 0; i < g_gen.num_lines; i++)
  {
    printf("    %s\n", g_gen.code[i]);
  }

  printf("    return;\n");
  printf("}\n");
}