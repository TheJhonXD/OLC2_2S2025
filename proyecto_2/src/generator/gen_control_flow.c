#include "gen_control_flow.h"
#include "gen_expressions.h"
#include "gen_statements.h"
#include "gen_utils.h"
#include "ast/sentencias/if.h"

// If
void generate_if(CodeGenerator *gen, NodoBase *if_stmt)
{
  If *i = (If *)if_stmt;

  // Obtener label para salto
  int label_end = get_next_label(gen);

  fprintf(gen->output_file, "    # If\n");

  // Evaluar condicion
  int reg = allocate_register(gen->reg_manager);
  generate_expression(gen, i->condicion, reg);

  // Si es falso, saltar al final
  fprintf(gen->output_file, "    cmp x%d, #0\n", reg);
  fprintf(gen->output_file, "    beq L%d\n", label_end);
  free_register(gen->reg_manager, reg);

  // Bloque verdadero
  generate_statement(gen, i->bloque);

  // Label de salida
  fprintf(gen->output_file, "L%d:\n", label_end);
}

// While TODO
void generate_while(CodeGenerator *gen, NodoBase *while_stmt)
{
  fprintf(gen->output_file, "    # While - TODO\n");
}

// For TODO
void generate_for(CodeGenerator *gen, NodoBase *for_stmt)
{
  fprintf(gen->output_file, "    # For - TODO\n");
}
