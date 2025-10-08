#include "gen_expressions.h"
#include "gen_utils.h"
#include "ast/expresiones/primitivos.h"
#include "ast/expresiones/operacion.h"
#include "ast/expresiones/variable.h"
#include <string.h>

// Generar codigo para una expresion
// El resultado se deja en el registro target_reg
void generate_expression(CodeGenerator *gen, NodoBase *expr, int target_reg)
{
  if (expr == NULL)
    return;

  // Ver que tipo de expresion es y generar el codigo apropiado
  if (strcmp(expr->nombre, "Primitive") == 0) // Numeros, strings, booleanos
  {
    generate_primitive(gen, expr, target_reg);
  }
  else if (strcmp(expr->nombre, "Variable") == 0)
  {
    generate_variable(gen, expr, target_reg);
  }
  else if (strcmp(expr->nombre, "Operation") == 0) // Operaciones (suma resta multiplicacion, etc)
  {
    generate_operation(gen, expr, target_reg);
  }
  else if (strcmp(expr->nombre, "CallFunc") == 0)
  {
    fprintf(gen->output_file, "    # TODO: CallFunc\n");
  }
}

// Generar codigo para valores puntuales es decir: 5, "hola", true
void generate_primitive(CodeGenerator *gen, NodoBase *prim, int target_reg)
{
  Primitive *p = (Primitive *)prim;
  Symbol s = p->s;

  if (s.tipo == T_INTEGER)
  {
    // Para numero entero: mov x1, #10
    fprintf(gen->output_file, "    mov x%d, #%d\n", target_reg, s.val.i);
  }
  else if (s.tipo == T_BOOLEAN)
  {
    // Para booleano: true = 1, false = 0
    fprintf(gen->output_file, "    mov x%d, #%d\n", target_reg, s.val.b ? 1 : 0);
  }
  else if (s.tipo == T_STRING)
  {
    // Los strings se guardarán en la seccion .data, luego cargar su direccion
    add_string_literal(gen, s.val.s);
    fprintf(gen->output_file, "    adr x%d, str_%d\n", target_reg, gen->string_count - 1);
  }
  else if (s.tipo == T_CHAR)
  {
    // Se convierte a su valor ascii
    fprintf(gen->output_file, "    mov x%d, #%d\n", target_reg, (int)s.val.c);
  }
  else if (s.tipo == T_FLOAT)
  {
    // Para floats/double lo guardo en la sección .data y se carga su valor
    int float_idx = gen->float_count;
    gen->float_literals[gen->float_count++] = s.val.f;
    
    fprintf(gen->output_file, "    adr x%d, float_%d\n", target_reg, float_idx);
    fprintf(gen->output_file, "    ldr x%d, [x%d]\n", target_reg, target_reg);
  }
  else
  {
    fprintf(gen->output_file, "    mov x%d, #0\n", target_reg);
  }
}

// Leer el valor de una variable desde memoria
void generate_variable(CodeGenerator *gen, NodoBase *var, int target_reg)
{
  Variable *v = (Variable *)var;

  // Buscar donde esta guardada la variable
  int offset = buscar_variable(gen, v->id);

  if (offset == -1)
  {
    fprintf(gen->output_file, "    # ERROR: Variable %s no existe\n", v->id);
    fprintf(gen->output_file, "    mov x%d, #0\n", target_reg);
    return;
  }

  // ldr = load register (cargar desde memoria al registro)
  // [sp, #offset] = stack pointer + offset
  fprintf(gen->output_file, "    ldr x%d, [sp, #%d]\n", target_reg, offset);
}

// Generar operaciones aritmeticas, comparaciones, logicas
void generate_operation(CodeGenerator *gen, NodoBase *op, int target_reg)
{
  Operation *o = (Operation *)op;

  // Detectar si la operacion hace alguna operaicon con floats
  TipoExpresion tipo = get_expression_type(gen, op);
  int is_float = (tipo == T_FLOAT);

  // Dos registros , uno para cada lado de la operacion
  int reg_izq = allocate_register(gen->reg_manager);
  int reg_der = allocate_register(gen->reg_manager);

  // Calcular el lado izquierdo y derecho
  generate_expression(gen, o->izq, reg_izq);
  generate_expression(gen, o->der, reg_der);

  // Operaciones aritmeticas basicas
  if (strcmp(o->op, "+") == 0)
  {
    if (is_float)
    {
      // Mover a registros d, operar y devolver a x
      fprintf(gen->output_file, "    fmov d0, x%d\n", reg_izq);
      fprintf(gen->output_file, "    fmov d1, x%d\n", reg_der);
      fprintf(gen->output_file, "    fadd d0, d0, d1\n");
      fprintf(gen->output_file, "    fmov x%d, d0\n", target_reg);
    }
    else
    {
      // add = sumar enteros
      fprintf(gen->output_file, "    add x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
    }
  }
  else if (strcmp(o->op, "-") == 0)
  {
    if (is_float)
    {
      fprintf(gen->output_file, "    fmov d0, x%d\n", reg_izq);
      fprintf(gen->output_file, "    fmov d1, x%d\n", reg_der);
      fprintf(gen->output_file, "    fsub d0, d0, d1\n");
      fprintf(gen->output_file, "    fmov x%d, d0\n", target_reg);
    }
    else
    {
      // sub = restar enteros
      fprintf(gen->output_file, "    sub x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
    }
  }
  else if (strcmp(o->op, "*") == 0)
  {
    if (is_float)
    {
      fprintf(gen->output_file, "    fmov d0, x%d\n", reg_izq);
      fprintf(gen->output_file, "    fmov d1, x%d\n", reg_der);
      fprintf(gen->output_file, "    fmul d0, d0, d1\n");
      fprintf(gen->output_file, "    fmov x%d, d0\n", target_reg);
    }
    else
    {
      // mul = multiplicar enteros
      fprintf(gen->output_file, "    mul x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
    }
  }
  else if (strcmp(o->op, "/") == 0)
  {
    if (is_float)
    {
      fprintf(gen->output_file, "    fmov d0, x%d\n", reg_izq);
      fprintf(gen->output_file, "    fmov d1, x%d\n", reg_der);
      fprintf(gen->output_file, "    fdiv d0, d0, d1\n");
      fprintf(gen->output_file, "    fmov x%d, d0\n", target_reg);
    }
    else
    {
      // sdiv = division con signo (signed division)
      fprintf(gen->output_file, "    sdiv x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
    }
  }
  else if (strcmp(o->op, "%") == 0)
  {
    // Modulo o el residuo de la division
    // arm64 no tiene la operacion de modulo, se calcula así: a % b = a - (a/b)*b
    fprintf(gen->output_file, "    sdiv x9, x%d, x%d\n", reg_izq, reg_der);   // x9 = a / b
    fprintf(gen->output_file, "    mul x9, x9, x%d\n", reg_der);              // x9 = (a/b) * b
    fprintf(gen->output_file, "    sub x%d, x%d, x9\n", target_reg, reg_izq); // target = a - x9
  }
  else if (strcmp(o->op, "unario") == 0)
  {
    // Unario -x
    fprintf(gen->output_file, "    neg x%d, x%d\n", target_reg, reg_izq);
  }
  // Operaciones de comparacion retornan 1 o 0
  else if (strcmp(o->op, ">") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der); // Se realiza la comparacion
    fprintf(gen->output_file, "    cset x%d, gt\n", target_reg);       // gt = greater than
  }
  else if (strcmp(o->op, "<") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, lt\n", target_reg); // lt = less than
  }
  else if (strcmp(o->op, ">=") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, ge\n", target_reg); // ge = greater or equal
  }
  else if (strcmp(o->op, "<=") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, le\n", target_reg); // le = less or equal
  }
  else if (strcmp(o->op, "==") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, eq\n", target_reg); // eq = equal
  }
  else if (strcmp(o->op, "!=") == 0)
  {
    fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
    fprintf(gen->output_file, "    cset x%d, ne\n", target_reg); // ne = not equal
  }
  // Operaciones logicas
  else if (strcmp(o->op, "&&") == 0)
  {
    // AND
    fprintf(gen->output_file, "    and x%d, x%d, x%d\n", target_reg, reg_izq, reg_der);
  }
  else if (strcmp(o->op, "||") == 0)
  {
    // OR
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

// Funcion para detectar el tipo de una expresion
TipoExpresion get_expression_type(CodeGenerator *gen, NodoBase *expr)
{
  if (expr == NULL)
    return T_INTEGER;

  if (strcmp(expr->nombre, "Primitive") == 0)
  {
    Primitive *p = (Primitive *)expr;
    return p->s.tipo;
  }
  else if (strcmp(expr->nombre, "Variable") == 0)
  {
    Variable *v = (Variable *)expr;
    int offset = buscar_variable(gen, v->id);
    if (offset != -1)
    {
      for (int i = 0; i < gen->var_table->count; i++)
      {
        if (gen->var_table->vars[i].offset == offset)
        {
          return gen->var_table->vars[i].tipo;
        }
      }
    }
  }
  else if (strcmp(expr->nombre, "Operation") == 0)
  {
    Operation *o = (Operation *)expr;
    // Si alguno de los operandos es float entonces toda la operacion es float al final
    TipoExpresion tipo_izq = get_expression_type(gen, o->izq);
    TipoExpresion tipo_der = get_expression_type(gen, o->der);
    if (tipo_izq == T_FLOAT || tipo_der == T_FLOAT)
    {
      return T_FLOAT;
    }
  }

  return T_INTEGER;
}
