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

// Agregar una variable a la tabla de simbolos
// Retorna el offset (posicion en memoria) donde se va a guardar
int agregar_variable(CodeGenerator *gen, char *nombre, TipoExpresion tipo)
{
  // Primero revisar si ya existe (para evitar duplicados)
  for (int i = 0; i < gen->var_table->count; i++)
  {
    if (strcmp(gen->var_table->vars[i].nombre, nombre) == 0)
    {
      return gen->var_table->vars[i].offset; // Ya existe, retornar su offset
    }
  }

  int indice = gen->var_table->count;

  // Calcular donde va en memoria
  // Cada variable ocupa 16 bytes porque ARM64 requiere alineamiento de 16
  // Variable 0 -> offset 0, variable 1 -> offset 16, variable 2 -> offset 32, etc
  int offset = indice * 16;

  gen->var_table->vars[indice].nombre = strdup(nombre);
  gen->var_table->vars[indice].offset = offset;
  gen->var_table->vars[indice].tipo = tipo;
  gen->var_table->count++;

  return offset;
}

// Buscar variable por nombre
int buscar_variable(CodeGenerator *gen, char *nombre)
{
  for (int i = 0; i < gen->var_table->count; i++)
  {
    if (strcmp(gen->var_table->vars[i].nombre, nombre) == 0)
    {
      return gen->var_table->vars[i].offset;
    }
  }
  return -1;
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
