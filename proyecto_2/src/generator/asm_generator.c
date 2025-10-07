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

    // Inicializar tabla de variables
    gen->var_table->count = 0;

    write_header(gen);
    return gen;
}
// Escribe el inicio del archivo
void write_header(CodeGenerator *gen)
{
    // .global _start indica que _start es el punto de entrada del programa
    fprintf(gen->output_file, ".global _start\n");
    // .text es la seccion donde va el codigo ejecutable
    fprintf(gen->output_file, ".text\n\n");

    // Esta es una funcion auxiliar que se llama cada vez que se haga un print
    fprintf(gen->output_file, "printValue:\n");
    fprintf(gen->output_file, "    # x1 = valor a imprimir\n");
    fprintf(gen->output_file, "    mov x8, #64\n"); // syscall 64 = write
    fprintf(gen->output_file, "    mov x0, #1\n");  // fd = 1 (stdout)
    fprintf(gen->output_file, "    svc #0\n");      // ejecutar syscall
    fprintf(gen->output_file, "    ret\n\n");       // ret = return (volver)
}

// Generar todo el programa
void generate_program(CodeGenerator *gen, NodoBase *root)
{
    // _start es donde empieza a ejecutar el programa
    fprintf(gen->output_file, "_start:\n");

    // Reservar espacio en el stack para todas las variables (quemado para 100 variables de 16 bytes)
    fprintf(gen->output_file, "    sub sp, sp, #1600\n");

    generate_statement(gen, root);
    write_footer(gen);
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