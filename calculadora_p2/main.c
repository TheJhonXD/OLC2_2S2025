#include <stdio.h>
#include <stdlib.h>
#include "generador/generador.h"

// Variables del parser
extern int yyparse(void);
extern FILE *yyin;

int main(int argc, char **argv)
{
    // Inicializar generador
    initGenerator();

    // Abrir archivo
    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (!yyin)
        {
            fprintf(stderr, "Error: No se puede abrir el archivo %s\n", argv[1]);
            return 1;
        }
    }

    // Parsear y generar código
    if (yyparse() == 0)
    {
        printFinalCode();
    }
    else
    {
        printf("ERROR en la compilación\n");
    }

    // Cerrar archivo
    if (yyin && yyin != stdin)
    {
        fclose(yyin);
    }

    return 0;
}