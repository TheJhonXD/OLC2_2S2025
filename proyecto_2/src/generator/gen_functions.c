#include "gen_functions.h"
#include "gen_expressions.h"
#include "gen_utils.h"

// Declaracion de funcion TODO
void generate_function_decl(CodeGenerator *gen, NodoBase *func)
{
  fprintf(gen->output_file, "    # Function declaration - TODO\n");
}

// Llamada a funcion TODO
void generate_function_call(CodeGenerator *gen, NodoBase *call, int target_reg)
{
  fprintf(gen->output_file, "    # Function call - TODO\n");
}

// Return TODO
void generate_return(CodeGenerator *gen, NodoBase *ret)
{
  fprintf(gen->output_file, "    # Return - TODO\n");
}
