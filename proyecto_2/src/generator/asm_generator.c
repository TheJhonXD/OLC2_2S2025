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
    fprintf(gen->output_file, "    # x1 = valor a imprimir (número entero)\n");
    fprintf(gen->output_file, "    stp x29, x30, [sp, #-16]!\n");  // Guardar registros
    fprintf(gen->output_file, "    mov x29, sp\n");
    fprintf(gen->output_file, "    sub sp, sp, #32\n");             // Espacio para buffer
    fprintf(gen->output_file, "\n");
    fprintf(gen->output_file, "    # Convertir número a string\n");
    fprintf(gen->output_file, "    mov x2, x1\n");                  // Copiar número
    fprintf(gen->output_file, "    add x3, sp, #16\n");             // Puntero al buffer
    fprintf(gen->output_file, "    mov x4, #10\n");                 // Divisor = 10
    fprintf(gen->output_file, "    mov x5, #0\n");                  // Contador de dígitos
    fprintf(gen->output_file, "\n");
    fprintf(gen->output_file, "convert_loop:\n");
    fprintf(gen->output_file, "    udiv x6, x2, x4\n");             // x6 = número / 10
    fprintf(gen->output_file, "    msub x7, x6, x4, x2\n");         // x7 = número %% 10 (resto)
    fprintf(gen->output_file, "    add x7, x7, #48\n");             // Convertir a ASCII ('0' = 48)
    fprintf(gen->output_file, "    strb w7, [x3, x5]\n");           // Guardar dígito
    fprintf(gen->output_file, "    add x5, x5, #1\n");              // Incrementar contador
    fprintf(gen->output_file, "    mov x2, x6\n");                  // número = número / 10
    fprintf(gen->output_file, "    cbnz x2, convert_loop\n");       // Si número != 0, continuar
    fprintf(gen->output_file, "\n");
    fprintf(gen->output_file, "    # Invertir string (los dígitos quedan al revés)\n");
    fprintf(gen->output_file, "    mov x6, #0\n");                  // Índice inicio
    fprintf(gen->output_file, "    sub x7, x5, #1\n");              // Índice fin
    fprintf(gen->output_file, "\n");
    fprintf(gen->output_file, "reverse_loop:\n");
    fprintf(gen->output_file, "    cmp x6, x7\n");
    fprintf(gen->output_file, "    bge print_number\n");
    fprintf(gen->output_file, "    ldrb w8, [x3, x6]\n");           // Cargar byte inicio
    fprintf(gen->output_file, "    ldrb w9, [x3, x7]\n");           // Cargar byte fin
    fprintf(gen->output_file, "    strb w9, [x3, x6]\n");           // Intercambiar
    fprintf(gen->output_file, "    strb w8, [x3, x7]\n");
    fprintf(gen->output_file, "    add x6, x6, #1\n");
    fprintf(gen->output_file, "    sub x7, x7, #1\n");
    fprintf(gen->output_file, "    b reverse_loop\n");
    fprintf(gen->output_file, "\n");
    fprintf(gen->output_file, "print_number:\n");
    fprintf(gen->output_file, "    mov x8, #64\n");                 // syscall write
    fprintf(gen->output_file, "    mov x0, #1\n");                  // fd = stdout
    fprintf(gen->output_file, "    mov x1, x3\n");                  // buffer = puntero al string
    fprintf(gen->output_file, "    mov x2, x5\n");                  // longitud = número de dígitos
    fprintf(gen->output_file, "    svc #0\n");
    fprintf(gen->output_file, "\n");
    fprintf(gen->output_file, "    add sp, sp, #32\n");             // Limpiar buffer
    fprintf(gen->output_file, "    ldp x29, x30, [sp], #16\n");     // Restaurar registros
    fprintf(gen->output_file, "    ret\n\n");                       // Retornar

    // Función para imprimir strings
    fprintf(gen->output_file, "printString:\n");
    fprintf(gen->output_file, "    # x1 = puntero al string\n");
    fprintf(gen->output_file, "    stp x29, x30, [sp, #-16]!\n");
    fprintf(gen->output_file, "    mov x29, sp\n");
    fprintf(gen->output_file, "\n");
    fprintf(gen->output_file, "    # Calcular longitud del string\n");
    fprintf(gen->output_file, "    mov x2, x1\n");
    fprintf(gen->output_file, "    mov x3, #0\n");
    fprintf(gen->output_file, "strlen_loop:\n");
    fprintf(gen->output_file, "    ldrb w4, [x2, x3]\n");
    fprintf(gen->output_file, "    cbz w4, print_str\n");
    fprintf(gen->output_file, "    add x3, x3, #1\n");
    fprintf(gen->output_file, "    b strlen_loop\n");
    fprintf(gen->output_file, "\n");
    fprintf(gen->output_file, "print_str:\n");
    fprintf(gen->output_file, "    mov x8, #64\n");
    fprintf(gen->output_file, "    mov x0, #1\n");
    fprintf(gen->output_file, "    mov x2, x3\n");
    fprintf(gen->output_file, "    svc #0\n");
    fprintf(gen->output_file, "\n");
    fprintf(gen->output_file, "    ldp x29, x30, [sp], #16\n");
    fprintf(gen->output_file, "    ret\n\n");
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