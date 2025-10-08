#include "gen_statements.h"
#include "gen_expressions.h"
#include "gen_utils.h"
#include "gen_control_flow.h"
#include "ast/sentencias/declaration.h"
#include "ast/sentencias/assigment.h"
#include "ast/sentencias/print.h"
#include "ast/sentencias/block.h"
#include "ast/expresiones/primitivos.h"
#include <string.h>

// Generar codigo para cualquier sentencia
// Basicamente revisa que tipo de sentencia es y llama a la funcion correcta
void generate_statement(CodeGenerator *gen, NodoBase *stmt)
{
  if (stmt == NULL)
    return;

  // Comparar el nombre del nodo para saber que tipo de sentencia es
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
  else if (strcmp(stmt->nombre, "IF") == 0)
  {
    generate_if(gen, stmt);
  }
  else if (strcmp(stmt->nombre, "Function") == 0)
  {
    fprintf(gen->output_file, "    # TODO: Function\n");
  }
}

// Generar un bloque de sentencias, las que van dentor de llaves
void generate_block(CodeGenerator *gen, Block *b)
{
  StmtCell *c = b->head; // Empezar desde la primera sentencia
  while (c != NULL)
  {
    generate_statement(gen, c->stmt);
    c = c->next; // Avanzar a la siguiente sentencia
  }
}

// Declarar una variable
void generate_declaration(CodeGenerator *gen, NodoBase *decl)
{
  Declaration *d = (Declaration *)decl;

  fprintf(gen->output_file, "    # Declaracion: %s\n", d->id);

  // Registrar la variable en la tabla para saber donde esta en memoria
  // Retorna el offset donde se va a guardar (0, 16, 32, etc)
  int offset = agregar_variable(gen, d->id, d->tipo);

  // Pedir un registro para trabajar
  int reg = allocate_register(gen->reg_manager);

  // Si tiene valor inicial (int x = 5) generar codigo para calcularlo
  if (d->expr != NULL)
  {
    generate_expression(gen, d->expr, reg); // Resultado queda en el registro
  }
  else
  {
    // Si no tiene valor, inicializar en 0
    fprintf(gen->output_file, "    mov x%d, #0\n", reg);
  }

  // Guardar el valor en memoria usando str (store)
  // [sp, #offset], es decir direccion = stack pointer + offset
  fprintf(gen->output_file, "    str x%d, [sp, #%d]\n", reg, offset);

  // Liberar el registro para que se pueda reutilizar
  free_register(gen->reg_manager, reg);
}

// Asignar un nuevo valor a una variable existente
void generate_assignment(CodeGenerator *gen, NodoBase *assign)
{
  Assigment *a = (Assigment *)assign;

  fprintf(gen->output_file, "    # Asignacion: %s\n", a->id);

  // Buscar donde esta la variable en memoria
  int offset = buscar_variable(gen, a->id);

  if (offset == -1)
  {
    // Si la variable no existe se marca error y se sale
    fprintf(gen->output_file, "    # ERROR: Variable %s no existe\n", a->id);
    return;
  }

  // Calcular el nuevo valor y guardarlo en la misma posicion
  int reg = allocate_register(gen->reg_manager);
  generate_expression(gen, a->expr, reg);
  fprintf(gen->output_file, "    str x%d, [sp, #%d]\n", reg, offset);
  free_register(gen->reg_manager, reg);
}

// Print
void generate_print(CodeGenerator *gen, NodoBase *print)
{
  Print *p = (Print *)print;

  fprintf(gen->output_file, "    # Print\n");

  // Verificar si es un string literal
  int is_string = 0;
  if (strcmp(p->expr->nombre, "Primitive") == 0)
  {
    Primitive *prim = (Primitive *)p->expr;
    if (prim->s.tipo == T_STRING)
    {
      is_string = 1;
    }
  }

  // Calcular el valor a imprimir
  int reg = allocate_register(gen->reg_manager);
  generate_expression(gen, p->expr, reg);

  // Llamar a la funcion printValue o printString segun el tipo
  fprintf(gen->output_file, "    mov x1, x%d\n", reg);
  if (is_string)
  {
    fprintf(gen->output_file, "    bl printString\n");
  }
  else
  {
    fprintf(gen->output_file, "    bl printValue\n"); // bl = branch and link, es decir llamar a la función
  }

  // Esto es un syscall write
  fprintf(gen->output_file, "    mov x8, #64\n");     // syscall 64 = write
  fprintf(gen->output_file, "    mov x0, #1\n");      // fd = 1 (stdout)
  fprintf(gen->output_file, "    adr x1, newline\n"); // direccion del string "\n"
  fprintf(gen->output_file, "    mov x2, #1\n");      // longitud = 1
  fprintf(gen->output_file, "    svc #0\n");          // ejecutar syscall

  free_register(gen->reg_manager, reg);
}
