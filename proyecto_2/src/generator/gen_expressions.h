#ifndef GEN_EXPRESSIONS_H
#define GEN_EXPRESSIONS_H

#include "asm_generator.h"

// Generar expresiones
void generate_expression(CodeGenerator *gen, NodoBase *expr, int target_reg);

// Expresiones especificas
void generate_primitive(CodeGenerator *gen, NodoBase *prim, int target_reg);
void generate_variable(CodeGenerator *gen, NodoBase *var, int target_reg);
void generate_operation(CodeGenerator *gen, NodoBase *op, int target_reg);
void generate_string_valueof(CodeGenerator *gen, NodoBase *expr, int target_reg);

// Auxiliar para detectar tipo de expresion
TipoExpresion get_expression_type(CodeGenerator *gen, NodoBase *expr);

#endif
