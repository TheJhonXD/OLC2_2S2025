#include "gen_utils.h"
#include <string.h>
#include <stdlib.h>

// Pedir un registro libre para usarlo
// ARM64 tiene registros x0 a x30, buscamos uno que no este ocupado
int allocate_register(RegisterManager *manager)
{
  // Empezamos en 1 porque x0 se usa para cosas especiales
  for (int i = 1; i < 31; i++)
  {
    if (manager->x_registers[i] == 0) // 0 = libre, 1 = ocupado
    {
      manager->x_registers[i] = 1; // Marcar como ocupado
      return i;                    // Retornar el numero del registro
    }
  }
  return -1; // No hay registros libres (esto seria un error grave)
}

// Marcar un registro como libre para que se pueda volver a usar
void free_register(RegisterManager *manager, int reg)
{
  if (reg >= 0 && reg < 31)
  {
    manager->x_registers[reg] = 0; // 0 = libre
  }
}

// Agregar una variable al entorno actual
// Retorna el offset (posicion en memoria) donde se va a guardar
int agregar_variable(CodeGenerator *gen, char *nombre, TipoExpresion tipo)
{
  // Calcular el offset actual
  // Cada variable ocupa 16 bytes porque ARM64 requiere alineamiento de 16
  int offset = gen->current_env->offset;

  // Crear el Symbol para guardar en el entorno
  Symbol sym;
  sym.tipo = tipo;
  sym.val.i = offset; // Guardamos el offset en el campo val.i
  sym.lin = 0;
  sym.col = 0;

  // Guardar la variable en el entorno actual
  Env_SaveVariable(gen->current_env, nombre, sym);

  // Incrementar el offset del entorno para la siguiente variable
  gen->current_env->offset += 16;

  return offset;
}

// Buscar variable por nombre en el entorno actual y sus padres
int buscar_variable(CodeGenerator *gen, char *nombre)
{
  // Buscar en el entorno actual y padres
  Symbol sym = Env_GetVariable(gen->current_env, nombre);

  // Si no se encuentra, Env_GetVariable retorna un Symbol con tipo NULL
  // En ese caso retornamos -1
  if (sym.tipo == T_NULL)
  {
    return -1;
  }

  // El offset está guardado en sym.val.i
  return sym.val.i;
}

// Agregar string literal
void add_string_literal(CodeGenerator *gen, const char *str)
{
  if (gen->string_count < 100)
  {
    gen->string_literals[gen->string_count] = malloc(strlen(str) + 1);
    strcpy(gen->string_literals[gen->string_count], str);
    gen->string_count++;
  }
}

// Obtener siguiente label
int get_next_label(CodeGenerator *gen)
{
  return gen->reg_manager->label_counter++;
}
