#ifndef GEN_UTILS_H
#define GEN_UTILS_H

#include "asm_generator.h"

// Manejo de registros
int allocate_register(RegisterManager *manager);
void free_register(RegisterManager *manager, int reg);

// Manejo de variables
int agregar_variable(CodeGenerator *gen, char *nombre, TipoExpresion tipo);
int buscar_variable(CodeGenerator *gen, char *nombre);

// Strings literales
void add_string_literal(CodeGenerator *gen, const char *str);

// Labels
int get_next_label(CodeGenerator *gen);

#endif
