#ifndef GEN_CONTROL_FLOW_H
#define GEN_CONTROL_FLOW_H

#include "asm_generator.h"

// Control de flujo
void generate_if(CodeGenerator *gen, NodoBase *if_stmt);
void generate_while(CodeGenerator *gen, NodoBase *while_stmt);
void generate_for(CodeGenerator *gen, NodoBase *for_stmt);

#endif
