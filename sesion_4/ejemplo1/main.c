#include <stdio.h>
#include <stdlib.h>
#include "entorno/ast.h"
#include "ast/AbstractTerminal.h"
#include "entorno/environment.h"

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
        (void)NodoBase_Ejecutar(g_root, &ast, &global);

    /* imprimir salida o errores */
    if (ast.errors[0])
    {
        fputs(ast.prints, stdout);
        fputs(ast.errors, stderr);
    }
    else
    {
        fputs(ast.prints, stdout);
    }

    /* liberar nodos (cada uno tiene el metodo para destruirse) */
    if (g_root)
        NodoBase_Destruir(g_root);
    Env_free(&global);

    return 0;
}
