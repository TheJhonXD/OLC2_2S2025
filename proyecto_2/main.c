#include <stdio.h>
#include <stdlib.h>
#include "entorno/ast.h"
#include "ast/AbstractTerminal.h"
#include "entorno/environment.h"
#include "generator/asm_generator.h"

/* del parser */
int yyparse(void);
extern FILE *yyin;

/* arreglo global de instrucciones construido por el parser */
extern struct NodoBase *g_root;

int main(int argc, char **argv)
{
    /* abrir archivo o stdin */
    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (!yyin)
        {
            perror("archivo");
            return 1;
        }
    }
    else
    {
        yyin = stdin;
    }

    /* parsear: llena g_code con instrucciones */
    if (yyparse() != 0)
    {
        fprintf(stderr, "Fallo en el parseo\n");
        if (yyin != stdin)
            fclose(yyin);
        return 1;
    }
    if (yyin != stdin)
        fclose(yyin);

    /* ejecutar: recorrer instrucciones */
    AST ast;
    AST_Init(&ast);
    Environment global;
    Env_init(&global, NULL, "GLOBAL");
    if (g_root)
    {
        // Comentado: solo generar código, no interpretar
        // (void)NodoBase_Ejecutar(g_root, &ast, &global);

        printf("Generando código ARM64...\n");

        // Determinar nombre del archivo de salida
        char output_filename[256] = "output.s";
        if (argc > 1)
        {
            // Si se pasó un archivo, cambiar extensión a .s
            char *dot = strrchr(argv[1], '.');
            if (dot)
            {
                size_t base_len = dot - argv[1];
                snprintf(output_filename, sizeof(output_filename), "%.*s.s", (int)base_len, argv[1]);
            }
        }

        // Inicializar generador y generar código
        CodeGenerator *gen = init_code_generator(output_filename);
        if (gen)
        {
            generate_program(gen, g_root); // g_root es el nodo raíz

            cleanup_code_generator(gen);
            printf("Código ARM64 generado en: %s\n", output_filename);
        }
        else
        {
            fprintf(stderr, "Error: No se pudo inicializar el generador de código\n");
        }
    }

    /* imprimir salida o errores */
    // if (ast.errors[0])
    // {
    //     fputs(ast.prints, stdout);
    //     fputs(ast.errors, stderr);
    // }
    // else
    // {
    //     fputs(ast.prints, stdout);
    // }

    /* liberar nodos (cada uno tiene el metodo para destruirse) */
    if (g_root)
        NodoBase_Destruir(g_root);
    Env_free(&global);

    return 0;
}
