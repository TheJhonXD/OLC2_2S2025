#include "gen_functions.h"
#include "gen_expressions.h"
#include "gen_utils.h"
#include "gen_statements.h"
#include "ast/sentencias/function.h"
#include "ast/expresiones/callFunc.h"
#include "ast/sentencias/return.h"
#include "ast/sentencias/params.h"
#include "ast/expresiones/args.h"
#include <string.h>

// Declaracion de funcion
void generate_function_decl(CodeGenerator *gen, NodoBase *func)
{
  Function *f = (Function *)func;

  fprintf(gen->output_file, "\n# Function: %s\n", f->id);
  fprintf(gen->output_file, "%s:\n", f->id);

  // Prólogo: guardar x29 (frame pointer) y x30 (link register)
  fprintf(gen->output_file, "    stp x29, x30, [sp, #-16]!\n");
  fprintf(gen->output_file, "    mov x29, sp\n");

  // Reservar espacio en el stack para variables locales
  fprintf(gen->output_file, "    sub sp, sp, #1600\n");

  // Crear un entorno para la función
  Environment *parent_env = gen->current_env;
  Environment func_env;
  Env_init(&func_env, NULL, f->id); // Funciones tienen scope independiente
  gen->current_env = &func_env;

  // Guardar el nombre de la función actual
  char *prev_function = gen->current_function;
  gen->current_function = f->id;

  // Guardar parámetros en el stack
  // Los parámetros vienen en x0-x7, los guardamos en el stack
  int param_index = 0;
  for (Param *p = f->params; p != NULL; p = p->next)
  {
    int offset = agregar_variable(gen, p->id, p->tipo);
    fprintf(gen->output_file, "    # Parametro: %s\n", p->id);
    fprintf(gen->output_file, "    str x%d, [sp, #%d]\n", param_index, offset);
    param_index++;
    if (param_index >= 8)
      break; // Se usan x0-x7 para parámetros si se necesitan más (cosas que no) se usa el stack
  }

  // Generar el cuerpo de la función
  generate_statement(gen, f->body);

  // Epílogo (en caso de que no haya return explícito)
  fprintf(gen->output_file, "%s_end:\n", f->id);
  fprintf(gen->output_file, "    add sp, sp, #1600\n");
  fprintf(gen->output_file, "    ldp x29, x30, [sp], #16\n");
  fprintf(gen->output_file, "    ret\n");

  // Restaurar el entorno y función anterior
  gen->current_env = parent_env;
  gen->current_function = prev_function;
  Env_free(&func_env);
}

// Llamada a funcion
void generate_function_call(CodeGenerator *gen, NodoBase *call, int target_reg)
{
  CallFunc *c = (CallFunc *)call;

  fprintf(gen->output_file, "    # Call function: %s\n", c->id);

  // Evaluar argumentos y pasarlos en x0-x7
  int arg_index = 0;
  for (ArgList *arg = c->args; arg != NULL; arg = arg->next)
  {
    if (arg_index >= 8)
      break; // Solo 8 argumentos en registros

    // Evaluar el argumento directamente en el registro xi
    generate_expression(gen, arg->expr, arg_index);
    arg_index++;
  }

  // Llamar a la función
  fprintf(gen->output_file, "    bl %s\n", c->id);

  // El resultado está en x0, moverlo al registro destino si es diferente
  if (target_reg != 0)
  {
    fprintf(gen->output_file, "    mov x%d, x0\n", target_reg);
  }
}

// Return
void generate_return(CodeGenerator *gen, NodoBase *ret)
{
  Return *r = (Return *)ret;

  fprintf(gen->output_file, "    # Return\n");

  // Si hay expresión, evaluarla y ponerla en x0
  if (r->expr)
  {
    generate_expression(gen, r->expr, 0); // Resultado en x0
  }
  else
  {
    fprintf(gen->output_file, "    mov x0, #0\n"); // Return sin valor
  }

  // Saltar al epílogo de la función
  if (gen->current_function)
  {
    fprintf(gen->output_file, "    b %s_end\n", gen->current_function);
  }
  else
  {
    // Si no estamos en una función, solo retornar
    fprintf(gen->output_file, "    add sp, sp, #1600\n");
    fprintf(gen->output_file, "    ldp x29, x30, [sp], #16\n");
    fprintf(gen->output_file, "    ret\n");
  }
}
