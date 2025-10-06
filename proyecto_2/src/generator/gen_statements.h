#ifndef GEN_STATEMENTS_H
#define GEN_STATEMENTS_H

#include "asm_generator.h"

// Generar sentencias
void generate_statement(CodeGenerator *gen, NodoBase *stmt);
void generate_block(CodeGenerator *gen, Block *b);

// Sentencias especificas
void generate_declaration(CodeGenerator *gen, NodoBase *decl);
void generate_assignment(CodeGenerator *gen, NodoBase *assign);
void generate_print(CodeGenerator *gen, NodoBase *print);

#endif
