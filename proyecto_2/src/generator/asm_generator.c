#include "asm_generator.h"
#include "gen_statements.h"
#include "gen_utils.h"
#include <stdlib.h>

// Inicializar generador
CodeGenerator *init_code_generator(const char *output_filename)
{
    CodeGenerator *gen = malloc(sizeof(CodeGenerator));
    gen->output_file = fopen(output_filename, "w");
    gen->reg_manager = malloc(sizeof(RegisterManager));
    gen->var_table = malloc(sizeof(TablaVariables));

    // Limpiar registros
    for (int i = 0; i < 31; i++)
    {
        gen->reg_manager->x_registers[i] = 0;
    }
    gen->reg_manager->sp_offset = 0;
    gen->reg_manager->label_counter = 0;
    gen->string_count = 0;
    gen->float_count = 0;

    // Inicializar tabla de variables
    gen->var_table->count = 0;

    write_header(gen);
    return gen;
}
// Escribe el inicio del archivo
void write_header(CodeGenerator *gen)
{
    fprintf(gen->output_file, ".global main\n");
    fprintf(gen->output_file, ".extern printf\n");
    fprintf(gen->output_file, ".text\n\n");

    // Para imprimir enteros
    fprintf(gen->output_file, "printValue:\n");
    fprintf(gen->output_file, "    stp x29, x30, [sp, #-16]!\n");
    fprintf(gen->output_file, "    mov x29, sp\n");
    fprintf(gen->output_file, "    mov x1, x0\n");              // Segundo argumento de printf
    fprintf(gen->output_file, "    adr x0, int_fmt\n");         // Formato %%d
    fprintf(gen->output_file, "    bl printf\n");
    fprintf(gen->output_file, "    ldp x29, x30, [sp], #16\n");
    fprintf(gen->output_file, "    ret\n\n");

    // PAra imprimir solo cadenas
    fprintf(gen->output_file, "printString:\n");
    fprintf(gen->output_file, "    stp x29, x30, [sp, #-16]!\n");
    fprintf(gen->output_file, "    mov x29, sp\n");
    fprintf(gen->output_file, "    mov x1, x0\n");              // String a imprimir
    fprintf(gen->output_file, "    adr x0, str_fmt\n");         // Formato %%s
    fprintf(gen->output_file, "    bl printf\n");
    fprintf(gen->output_file, "    ldp x29, x30, [sp], #16\n");
    fprintf(gen->output_file, "    ret\n\n");

    // Para imprimir double
    fprintf(gen->output_file, "printFloat:\n");
    fprintf(gen->output_file, "    stp x29, x30, [sp, #-16]!\n");
    fprintf(gen->output_file, "    mov x29, sp\n");
    fprintf(gen->output_file, "    fmov d0, x0\n");             // Mover bits del registro x0 a d0
    fprintf(gen->output_file, "    adr x0, float_fmt\n");       // Formato %%f
    fprintf(gen->output_file, "    bl printf\n");
    fprintf(gen->output_file, "    ldp x29, x30, [sp], #16\n");
    fprintf(gen->output_file, "    ret\n\n");
}

// Generar todo el programa
void generate_program(CodeGenerator *gen, NodoBase *root)
{
    // main es el punto de entrada cuando usamos libc
    fprintf(gen->output_file, "main:\n");
    fprintf(gen->output_file, "    stp x29, x30, [sp, #-16]!\n");
    fprintf(gen->output_file, "    mov x29, sp\n");

    // Reservar espacio en el stack para todas las variables
    fprintf(gen->output_file, "    sub sp, sp, #1600\n");

    generate_statement(gen, root);
    write_footer(gen);
}

// Footer
void write_footer(CodeGenerator *gen)
{
    fprintf(gen->output_file, "\n    # Salir\n");
    fprintf(gen->output_file, "    add sp, sp, #1600\n");
    fprintf(gen->output_file, "    ldp x29, x30, [sp], #16\n");
    fprintf(gen->output_file, "    mov x0, #0\n");
    fprintf(gen->output_file, "    ret\n\n");

    // Datos
    fprintf(gen->output_file, ".data\n");
    fprintf(gen->output_file, "int_fmt: .asciz \"%%d\"\n");
    fprintf(gen->output_file, "str_fmt: .asciz \"%%s\"\n");
    fprintf(gen->output_file, "float_fmt: .asciz \"%%f\"\n");
    fprintf(gen->output_file, "newline: .asciz \"\\n\"\n");

    // Strings
    for (int i = 0; i < gen->string_count; i++)
    {
        fprintf(gen->output_file, "str_%d: .asciz \"%s\"\n", i, gen->string_literals[i]);
    }

    // Floats
    for (int i = 0; i < gen->float_count; i++)
    {
        fprintf(gen->output_file, "float_%d: .double %f\n", i, gen->float_literals[i]);
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