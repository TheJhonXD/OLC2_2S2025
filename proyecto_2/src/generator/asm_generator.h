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

// Generador de codigo
typedef struct
{
    FILE *output_file;
    RegisterManager *reg_manager;
    char *string_literals[100];
    int string_count;
} CodeGenerator;

// Inicializar
CodeGenerator *init_code_generator(const char *output_filename);
void cleanup_code_generator(CodeGenerator *gen);

// Generar codigo
void generate_program(CodeGenerator *gen, NodoBase *root);
void generate_statement(CodeGenerator *gen, NodoBase *stmt);
void generate_expression(CodeGenerator *gen, NodoBase *expr, int target_reg);
void generate_block(CodeGenerator *gen, Block *b);

// Sentencias
void generate_declaration(CodeGenerator *gen, NodoBase *decl);
void generate_assignment(CodeGenerator *gen, NodoBase *assign);
void generate_print(CodeGenerator *gen, NodoBase *print);

// Expresiones
void generate_operation(CodeGenerator *gen, NodoBase *op, int target_reg);
void generate_primitive(CodeGenerator *gen, NodoBase *prim, int target_reg);
void generate_variable(CodeGenerator *gen, NodoBase *var, int target_reg);

// Helpers
void write_header(CodeGenerator *gen);
void write_footer(CodeGenerator *gen);
void add_string_literal(CodeGenerator *gen, const char *str);
int allocate_register(RegisterManager *manager);
void free_register(RegisterManager *manager, int reg);

#endif