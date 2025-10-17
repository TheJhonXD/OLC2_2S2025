#ifndef ASM_GENERATOR_H
#define ASM_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast/AbstractTerminal.h"
#include "ast/sentencias/block.h"
#include "entorno/environment.h"

// Manejo de registros
typedef struct
{
    int x_registers[31];
    int sp_offset;
    int label_counter;
} RegisterManager;

// Generador de codigo
typedef struct
{
    FILE *output_file;
    RegisterManager *reg_manager;
    Environment *current_env; // Entorno actual (scope actual)
    char *string_literals[100];
    int string_count;
    double float_literals[100]; // Mantener los floats
    int float_count;
    char *current_function; // Nombre de la función actual
} CodeGenerator;

// Funciones principales
CodeGenerator *init_code_generator(const char *output_filename);
void cleanup_code_generator(CodeGenerator *gen);
void generate_program(CodeGenerator *gen, NodoBase *root);
void write_header(CodeGenerator *gen);
void write_footer(CodeGenerator *gen);

#endif