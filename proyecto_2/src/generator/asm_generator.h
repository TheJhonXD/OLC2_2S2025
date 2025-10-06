#ifndef ASM_GENERATOR_H
#define ASM_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast/AbstractTerminal.h"
#include "ast/sentencias/block.h"

// Manejo de registros
typedef struct
{
    int x_registers[31];
    int sp_offset;
    int label_counter;
} RegisterManager;

// Info de una variable
typedef struct
{
    char *nombre;
    int offset;
    TipoExpresion tipo;
} VariableInfo;

// Tabla de variables
typedef struct
{
    VariableInfo vars[100];
    int count;
} TablaVariables;

// Generador de codigo
typedef struct
{
    FILE *output_file;
    RegisterManager *reg_manager;
    TablaVariables *var_table;
    char *string_literals[100];
    int string_count;
} CodeGenerator;

// Funciones principales
CodeGenerator *init_code_generator(const char *output_filename);
void cleanup_code_generator(CodeGenerator *gen);
void generate_program(CodeGenerator *gen, NodoBase *root);
void write_header(CodeGenerator *gen);
void write_footer(CodeGenerator *gen);

#endif