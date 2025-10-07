#include "gen_control_flow.h"
#include "gen_expressions.h"
#include "gen_statements.h"
#include "gen_utils.h"
#include "ast/sentencias/if.h"

// Generar if
void generate_if(CodeGenerator *gen, NodoBase *if_stmt)
{
  If *i = (If *)if_stmt;

  // Crea una etiqueta para saltar si la condicion es falsa, es decir un numero unico: L0, L1, L2
  int label_end = get_next_label(gen);

  fprintf(gen->output_file, "    # If\n");

  // Primero se evalua la condicion
  // El resultado queda en el registro (1 = true, 0 = false)
  int reg = allocate_register(gen->reg_manager);
  generate_expression(gen, i->condicion, reg);

  // cmp = compare (comparar el registro con 0)
  fprintf(gen->output_file, "    cmp x%d, #0\n", reg);
  // beq = branch if equal, es decir saltar a L# si es igual a 0, o sea, si es falso
  fprintf(gen->output_file, "    beq L%d\n", label_end);
  free_register(gen->reg_manager, reg);

  // Si la condicion es verdadera, ejecutar el bloque del if
  generate_statement(gen, i->bloque);

  // Poner la etiqueta al final para que salte aqui si es falso
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
