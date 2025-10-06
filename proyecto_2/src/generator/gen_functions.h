#ifndef GEN_FUNCTIONS_H
#define GEN_FUNCTIONS_H

#include "asm_generator.h"

// Funciones aún no se implementar, será para la siguiente clase
void generate_function_decl(CodeGenerator *gen, NodoBase *func);
void generate_function_call(CodeGenerator *gen, NodoBase *call, int target_reg);
void generate_return(CodeGenerator *gen, NodoBase *ret);

#endif
