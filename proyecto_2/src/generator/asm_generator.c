#include "asm_generator.h"
#include "ast/expresiones/primitivos.h"
#include "ast/sentencias/declaration.h"
#include "ast/sentencias/assigment.h"
#include "ast/expresiones/operacion.h"
#include "ast/sentencias/print.h"
#include "ast/sentencias/block.h"

// Inicializar generador
CodeGenerator *init_code_generator(const char *output_filename)
{
    CodeGenerator *gen = malloc(sizeof(CodeGenerator));
    gen->output_file = fopen(output_filename, "w");
    gen->reg_manager = malloc(sizeof(RegisterManager));

    // Limpiar registros
    for (int i = 0; i < 31; i++)
    {
        gen->reg_manager->x_registers[i] = 0;
    }
    gen->reg_manager->sp_offset = 0;
    gen->reg_manager->label_counter = 0;
    gen->string_count = 0;

    write_header(gen);
    return gen;
}

// Header del archivo
void write_header(CodeGenerator *gen)
{
    fprintf(gen->output_file, ".global _start\n");
    fprintf(gen->output_file, ".text\n\n");

    // Funcion para imprimir enteros
    fprintf(gen->output_file, "print_int:\n");
    fprintf(gen->output_file, "    mov x8, #64\n");
    fprintf(gen->output_file, "    mov x0, #1\n");
    fprintf(gen->output_file, "    svc #0\n");
    fprintf(gen->output_file, "    ret\n\n");

    // Funcion para imprimir strings
    fprintf(gen->output_file, "print_string:\n");
    fprintf(gen->output_file, "    mov x8, #64\n");
    fprintf(gen->output_file, "    mov x0, #1\n");
    fprintf(gen->output_file, "    svc #0\n");
    fprintf(gen->output_file, "    ret\n\n");
}

// Generar programa completo
void generate_program(CodeGenerator *gen, NodoBase *root)
{
    fprintf(gen->output_file, "_start:\n");
    generate_statement(gen, root);
    write_footer(gen);
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
}

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

// Declaracion de variable
void generate_declaration(CodeGenerator *gen, NodoBase *decl)
{
    Declaration *d = (Declaration *)decl;

    fprintf(gen->output_file, "    # Declaracion: %s\n", d->id);

    // Reservar espacio en stack
    gen->reg_manager->sp_offset += 8;
    fprintf(gen->output_file, "    sub sp, sp, #8\n");

    // Si tiene valor inicial
    if (d->expr)
    {
        int reg = allocate_register(gen->reg_manager);
        generate_expression(gen, d->expr, reg);
        fprintf(gen->output_file, "    str x%d, [sp, #0]\n", reg);
        free_register(gen->reg_manager, reg);
    }
    else
    {
        // Inicializar en 0
        fprintf(gen->output_file, "    mov x1, #0\n");
        fprintf(gen->output_file, "    str x1, [sp, #0]\n");
    }
}

// Asignacion
void generate_assignment(CodeGenerator *gen, NodoBase *assign)
{
    Assigment *a = (Assigment *)assign;

    fprintf(gen->output_file, "    # Asignacion: %s\n", a->id);

    int reg = allocate_register(gen->reg_manager);
    generate_expression(gen, a->expr, reg);
    fprintf(gen->output_file, "    str x%d, [sp, #0]\n", reg);
    free_register(gen->reg_manager, reg);
}

// Print
void generate_print(CodeGenerator *gen, NodoBase *print)
{
    Print *p = (Print *)print;

    fprintf(gen->output_file, "    # Print\n");

    int reg = allocate_register(gen->reg_manager);
    generate_expression(gen, p->expr, reg);

    // Llamar a print_int
    fprintf(gen->output_file, "    mov x1, x%d\n", reg);
    fprintf(gen->output_file, "    bl print_int\n");

    // Imprimir salto de linea
    fprintf(gen->output_file, "    mov x8, #64\n");
    fprintf(gen->output_file, "    mov x0, #1\n");
    fprintf(gen->output_file, "    adr x1, newline\n");
    fprintf(gen->output_file, "    mov x2, #1\n");
    fprintf(gen->output_file, "    svc #0\n");

    free_register(gen->reg_manager, reg);
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
    else if (strcmp(o->op, "unario") == 0)
    {
        fprintf(gen->output_file, "    neg x%d, x%d\n", target_reg, reg_izq);
    }
    else if (strcmp(o->op, ">") == 0)
    {
        fprintf(gen->output_file, "    cmp x%d, x%d\n", reg_izq, reg_der);
        fprintf(gen->output_file, "    cset x%d, gt\n", target_reg);
    }

    free_register(gen->reg_manager, reg_izq);
    free_register(gen->reg_manager, reg_der);
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

// Cargar variable
void generate_variable(CodeGenerator *gen, NodoBase *var, int target_reg)
{
    fprintf(gen->output_file, "    ldr x%d, [sp, #0]\n", target_reg);
}

// Obtener registro libre
int allocate_register(RegisterManager *manager)
{
    for (int i = 1; i < 31; i++)
    {
        if (manager->x_registers[i] == 0)
        {
            manager->x_registers[i] = 1;
            return i;
        }
    }
    return -1;
}

// Liberar registro
void free_register(RegisterManager *manager, int reg)
{
    if (reg >= 0 && reg < 31)
    {
        manager->x_registers[reg] = 0;
    }
}

// Footer
void write_footer(CodeGenerator *gen)
{
    fprintf(gen->output_file, "\n    # Salir\n");
    fprintf(gen->output_file, "    mov x8, #93\n");
    fprintf(gen->output_file, "    mov x0, #0\n");
    fprintf(gen->output_file, "    svc #0\n\n");

    // Datos
    fprintf(gen->output_file, ".data\n");
    fprintf(gen->output_file, "newline: .ascii \"\\n\"\n");

    // Strings
    for (int i = 0; i < gen->string_count; i++)
    {
        fprintf(gen->output_file, "str_%d: .asciz \"%s\"\n", i, gen->string_literals[i]);
    }
}

// Limpiar
void cleanup_code_generator(CodeGenerator *gen)
{
    if (gen->output_file)
    {
        fclose(gen->output_file);
    }
    if (gen->reg_manager)
    {
        free(gen->reg_manager);
    }
    free(gen);
}