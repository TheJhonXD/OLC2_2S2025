#include "gen_statements.h"
#include "gen_expressions.h"
#include "gen_utils.h"
#include "gen_control_flow.h"
#include "ast/sentencias/declaration.h"
#include "ast/sentencias/assigment.h"
#include "ast/sentencias/print.h"
#include "ast/sentencias/block.h"
#include <string.h>

// Generar sentencia
void generate_statement(CodeGenerator *gen, NodoBase *stmt)
{
  if (stmt == NULL)
    return;

  if (strcmp(stmt->nombre, "Declaration") == 0)
  {
    generate_declaration(gen, stmt);
  }
  else if (strcmp(stmt->nombre, "Assigment") == 0)
  {
    generate_assignment(gen, stmt);
  }
  else if (strcmp(stmt->nombre, "Print") == 0)
  {
    generate_print(gen, stmt);
  }
  else if (strcmp(stmt->nombre, "Block") == 0)
  {
    generate_block(gen, (Block *)stmt);
  }
  else if (strcmp(stmt->nombre, "If") == 0)
  {
    generate_if(gen, stmt);
  }
}

// Generar bloque de sentencias
void generate_block(CodeGenerator *gen, Block *b)
{
  StmtCell *c = b->head;
  while (c != NULL)
  {
    generate_statement(gen, c->stmt);
    c = c->next;
  }
}

// Declaracion de variable
void generate_declaration(CodeGenerator *gen, NodoBase *decl)
{
  Declaration *d = (Declaration *)decl;

  fprintf(gen->output_file, "    # Declaracion: %s\n", d->id);

  // Agregar a la tabla y obtener su offset
  int offset = agregar_variable(gen, d->id, d->tipo);

  // Reservar espacio (16 bytes para que esté alineado)
  fprintf(gen->output_file, "    sub sp, sp, #16\n");
  gen->reg_manager->sp_offset += 16;

  // Si tiene valor inicial
  if (d->expr)
  {
    int reg = allocate_register(gen->reg_manager);
    generate_expression(gen, d->expr, reg);
    // Guardar en el offset correcto
    fprintf(gen->output_file, "    str x%d, [sp, #%d]\n", reg, offset);
    free_register(gen->reg_manager, reg);
  }
  else
  {
    // Inicializar en 0
    fprintf(gen->output_file, "    mov x1, #0\n");
    fprintf(gen->output_file, "    str x1, [sp, #%d]\n", offset);
  }
}

// Asignacion
void generate_assignment(CodeGenerator *gen, NodoBase *assign)
{
  Assigment *a = (Assigment *)assign;

  fprintf(gen->output_file, "    # Asignacion: %s\n", a->id);

  // Buscar offset de la variable
  int offset = buscar_variable(gen, a->id);

  if (offset == -1)
  {
    fprintf(gen->output_file, "    # ERROR: Variable %s no existe\n", a->id);
    return;
  }

  int reg = allocate_register(gen->reg_manager);
  generate_expression(gen, a->expr, reg);
  // Guardar en el offset correcto
  fprintf(gen->output_file, "    str x%d, [sp, #%d]\n", reg, offset);
  free_register(gen->reg_manager, reg);
}

// Print
void generate_print(CodeGenerator *gen, NodoBase *print)
{
  Print *p = (Print *)print;

  fprintf(gen->output_file, "    # Print\n");

  int reg = allocate_register(gen->reg_manager);
  generate_expression(gen, p->expr, reg);

  // Imprimir el valor
  fprintf(gen->output_file, "    mov x1, x%d\n", reg);
  fprintf(gen->output_file, "    bl printValue\n");

  // Imprimir salto de linea
  fprintf(gen->output_file, "    mov x8, #64\n");
  fprintf(gen->output_file, "    mov x0, #1\n");
  fprintf(gen->output_file, "    adr x1, newline\n");
  fprintf(gen->output_file, "    mov x2, #1\n");
  fprintf(gen->output_file, "    svc #0\n");

  free_register(gen->reg_manager, reg);
}
