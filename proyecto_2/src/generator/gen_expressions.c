#include "gen_expressions.h"
#include "gen_utils.h"
#include "ast/expresiones/primitivos.h"
#include "ast/expresiones/operacion.h"
#include "ast/expresiones/variable.h"
#include <string.h>

// Generar expresion
void generate_expression(CodeGenerator *gen, NodoBase *expr, int target_reg)
{
  if (expr == NULL)
    return;

  if (strcmp(expr->nombre, "Primitivos") == 0)
  {
    generate_primitive(gen, expr, target_reg);
  }
  else if (strcmp(expr->nombre, "Variable") == 0)
  {
    generate_variable(gen, expr, target_reg);
  }
  else if (strcmp(expr->nombre, "Operation") == 0)
  {
    generate_operation(gen, expr, target_reg);
  }
}

// Primitivos
void generate_primitive(CodeGenerator *gen, NodoBase *prim, int target_reg)
{
  Primitive *p = (Primitive *)prim;
  Symbol s = p->s;

  if (s.tipo == T_INTEGER)
  {
    fprintf(gen->output_file, "    mov x%d, #%d\n", target_reg, s.val.i);
  }
  else if (s.tipo == T_BOOLEAN)
  {
    fprintf(gen->output_file, "    mov x%d, #%d\n", target_reg, s.val.b ? 1 : 0);
  }
  else if (s.tipo == T_STRING)
  {
    add_string_literal(gen, s.val.s);
    fprintf(gen->output_file, "    adr x%d, str_%d\n", target_reg, gen->string_count - 1);
  }
  else if (s.tipo == T_CHAR)
  {
    fprintf(gen->output_file, "    mov x%d, #%d\n", target_reg, (int)s.val.c);
  }
  else
  {
    fprintf(gen->output_file, "    mov x%d, #0\n", target_reg);
  }
}

// Cargar variable
void generate_variable(CodeGenerator *gen, NodoBase *var, int target_reg)
{
  Variable *v = (Variable *)var;

  // Buscar offset de la variable
  int offset = buscar_variable(gen, v->id);

  if (offset == -1)
  {
    fprintf(gen->output_file, "    # ERROR: Variable %s no existe\n", v->id);
    fprintf(gen->output_file, "    mov x%d, #0\n", target_reg);
    return;
  }

  // Cargar desde el offset correcto
  fprintf(gen->output_file, "    ldr x%d, [sp, #%d]\n", target_reg, offset);
}

// Operaciones
void generate_operation(CodeGenerator *gen, NodoBase *op, int target_reg)
{
  Operation *o = (Operation *)op;

  int reg_izq = allocate_register(gen->reg_manager);
  int reg_der = allocate_register(gen->reg_manager);

  generate_expression(gen, o->izq, reg_izq);
  generate_expression(gen, o->der, reg_der);

  // Segun la operacion
  if (strcmp(o->op, "+") == 0)
  {
    fprintf(gen->output_file, "    add x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
  }
  else if (strcmp(o->op, "-") == 0)
  {
    fprintf(gen->output_file, "    sub x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
  }
  else if (strcmp(o->op, "*") == 0)
  {
    fprintf(gen->output_file, "    mul x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
  }
  else if (strcmp(o->op, "/") == 0)
  {
    fprintf(gen->output_file, "    sdiv x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
  }
  else if (strcmp(o->op, "%") == 0)
  {
    // Modulo, como no existe se hace así: a % b = a - (a/b)*b
    fprintf(gen->output_file, "    sdiv x9, x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    mul x9, x9, x%d\n", reg_der);
    fprintf(gen->output_file, "    sub x%d, x%d, x9\n", target_reg, reg_izq);
  }
  else if (strcmp(o->op, "unario") == 0)
  {
    fprintf(gen->output_file, "    neg x%d, x%d\n", target_reg, reg_izq);
  }
  else if (strcmp(o->op, ">") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, gt\n", target_reg);
  }
  else if (strcmp(o->op, "<") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, lt\n", target_reg);
  }
  else if (strcmp(o->op, ">=") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, ge\n", target_reg);
  }
  else if (strcmp(o->op, "<=") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, le\n", target_reg);
  }
  else if (strcmp(o->op, "==") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, eq\n", target_reg);
  }
  else if (strcmp(o->op, "!=") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, ne\n", target_reg);
  }
  else if (strcmp(o->op, "&&") == 0)
  {
    fprintf(gen->output_file, "    and x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
  }
  else if (strcmp(o->op, "||") == 0)
  {
    fprintf(gen->output_file, "    orr x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
  }
  else if (strcmp(o->op, "!") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, #0\n", reg_izq);
    fprintf(gen->output_file, "    cset x%d, eq\n", target_reg);
  }

  free_register(gen->reg_manager, reg_izq);
  free_register(gen->reg_manager, reg_der);
}
