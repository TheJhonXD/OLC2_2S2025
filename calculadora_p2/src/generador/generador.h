#ifndef GENERADOR_H
#define GENERADOR_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_CODE_LINES 10000
#define MAX_TEMPS 1000

typedef struct
{
  int temporal;                   // Contador de temporales
  int label;                      // Contador de labels
  char code[MAX_CODE_LINES][256]; // Array de líneas de código
  int num_lines;                  // Número actual de líneas
  char temps[MAX_TEMPS][32];      // Lista de temporales generados
  int num_temps;                  // Número de temporales
} Generator;

// Variable global del generador
extern Generator g_gen;

void initGenerator();
char *newTemp();
char *newLabel();

// Instrucciones de agregar
void addExpression(const char *target, const char *left, const char *operator, const char * right);
void addAssign(const char *target, const char *value);
void addLabel(const char *label);
void addGoto(const char *label);
void addIf(const char *left, const char *operator, const char * right, const char *label);
void addPrint(const char *value);
void addComment(const char *comment);

// Generar código final
void printCode();      // solo genera el codigo de c3d
void printFinalCode(); // genera el codigo final en c

#endif // GENERADOR_H