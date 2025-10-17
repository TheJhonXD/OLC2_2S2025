#include "gen_control_flow.h"
#include "gen_expressions.h"
#include "gen_statements.h"
#include "gen_utils.h"
#include "ast/sentencias/if.h"
#include "entorno/environment.h"
#include <stdlib.h>

// Generar if con su propio entorno
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

  // Crear un nuevo entorno para el bloque del if
  // El padre es el entorno actual del generador
  Environment *parent_env = gen->current_env;
  Environment if_env;
  Env_init(&if_env, parent_env, "IF_STMT");
  gen->current_env = &if_env;

  // Generar el bloque del if con el nuevo entorno
  generate_statement(gen, i->bloque);

  // Restaurar el entorno padre
  gen->current_env = parent_env;

  // Liberar el entorno del if
  Env_free(&if_env);

  // Poner la etiqueta al final para que salte aqui si es falso
  fprintf(gen->output_file, "L%d:\n", label_end);
}

// While con su propio entorno
void generate_while(CodeGenerator *gen, NodoBase *while_stmt)
{
  fprintf(gen->output_file, "    # While - TODO\n");

  // TODO: Cuando implementes while, sigue esta estructura:
  // 1. Crear etiquetas para el inicio del loop y el final
  // 2. Etiqueta de inicio del loop
  // 3. Evaluar condición
  // 4. Si es falso, saltar al final
  // 5. Crear nuevo entorno para el bloque del while
  // 6. Ejecutar el bloque
  // 7. Restaurar entorno padre y liberar entorno del while
  // 8. Saltar de vuelta al inicio del loop
  // 9. Etiqueta de final del loop
}

// For con su propio entorno
void generate_for(CodeGenerator *gen, NodoBase *for_stmt)
{
  fprintf(gen->output_file, "    # For - TODO\n");

  // TODO: Cuando implementes for, sigue esta estructura:
  // 1. Crear nuevo entorno para el for (la variable de iteración vive aquí)
  // 2. Ejecutar inicialización
  // 3. Crear etiquetas para el inicio del loop y el final
  // 4. Etiqueta de inicio del loop
  // 5. Evaluar condición
  // 6. Si es falso, saltar al final
  // 7. Crear otro entorno hijo para el bloque interno del for
  // 8. Ejecutar el bloque
  // 9. Restaurar entorno del bloque interno
  // 10. Ejecutar incremento
  // 11. Saltar de vuelta al inicio del loop
  // 12. Etiqueta de final, restaurar entorno del for y liberarlo
}
