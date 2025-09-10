#include <stdio.h>
#include <stdlib.h>
#include "entorno/ast.h"
#include "ast/AbstractTerminal.h"
#include "entorno/environment.h"
#include "ast/expresiones/operacion.h"
#include "ast/expresiones/callFunc.h"
#include "ast/sentencias/block.h"
#include "ast/sentencias/function.h"
#include "ast/expresiones/primitivos.h"
#include "ast/sentencias/declaration.h"

/* del parser */
int yyparse(void);
extern FILE *yyin;

/* arreglo global de instrucciones construido por el parser */
extern struct NodoBase *g_root;

static int nodo_counter = 0;

void print_dot_nodes(NodoBase *node, int parent_id)
{
    int current_id = nodo_counter++;
    char escaped_name[64];
    strncpy(escaped_name, node->nombre, sizeof(escaped_name) - 1);
    printf("  node_%d [label=\"%s\\n[%d:%d]\"", current_id, escaped_name, node->lin, node->col);

    if (strstr(node->nombre, "Operation"))
    {
        Operation *op = (Operation *)node;
        printf(", shape=ellipse, color=blue, style=filled, fillcolor=lightblue");
        if (op->op)
        {
            printf(", xlabel=\"%s\"", op->op);
        }
    }
    else if (strstr(node->nombre, "CallFunc"))
    {
        CallFunc *call = (CallFunc *)node;
        printf(", shape=ellipse, color=blue, style=filled, fillcolor=lightblue");
        if (call->id)
        {
            printf(", xlabel=\"%s\"", call->id);
        }
    }
    else if (strstr(node->nombre, "Block"))
    {
        printf(", shape=ellipse, color=blue, style=filled, fillcolor=lightblue");
    }
    else if (strstr(node->nombre, "Primitive") || strstr(node->nombre, "Variable"))
    {
        printf(", shape=circle, color=orange, style=filled, fillcolor=lightyellow");
        if (strstr(node->nombre, "Primitive"))
        {
            char valor_string[64];
            Primitive *prim = (Primitive *)node;
            switch (prim->s.tipo)
            {
            case T_INTEGER:
                snprintf(valor_string, sizeof(valor_string), "%d", prim->s.val.i);
                break;
            case T_FLOAT:
                snprintf(valor_string, sizeof(valor_string), "%f", prim->s.val.f);
                break;
            case T_STRING:
                snprintf(valor_string, sizeof(valor_string), "\"%s\"", prim->s.val.s);
                break;
            case T_BOOLEAN:
                snprintf(valor_string, sizeof(valor_string), "%s", prim->s.val.b ? "true" : "false");
                break;
            case T_CHAR:
                snprintf(valor_string, sizeof(valor_string), "'%c'", prim->s.val.c);
                break;
            }
        }
    }
    else if (strstr(node->nombre, "Declaration"))
    {
        printf(", shape=circle, color=red, style=filled, fillcolor=lightcoral");
    }
    else if (strstr(node->nombre, "Assignment"))
    {
        printf(", shape=circle, color=brown, style=filled, fillcolor=wheat");
    }
    else if (strstr(node->nombre, "If"))
    {
        printf(", shape=circle, color=navy, style=filled, fillcolor=lightsteelblue");
    }

    else if (strstr(node->nombre, "Print"))
    {
        printf(", shape=circle, color=navy, style=filled, fillcolor=lightsteelblue");
    }
    else if (strstr(node->nombre, "Function"))
    {
        printf(", shape=circle, color=navy, style=filled, fillcolor=lightsteelblue");
    }

    printf("];\n");

    if (parent_id >= 0)
    {
        printf("  node_%d -> node_%d;\n", parent_id, current_id);
    }

    if (strstr(node->nombre, "Operation"))
    {
        Operation *op = (Operation *)node;
        if (op->izq)
            print_dot_nodes(op->izq, current_id);
        if (op->der)
            print_dot_nodes(op->der, current_id);
    }
    else if (strstr(node->nombre, "Block"))
    {
        Block *block = (Block *)node;
        if (block->head)
        {
            StmtCell *current = block->head;
            while (current)
            {
                print_dot_nodes(current->stmt, current_id);
                current = current->next;
            }
        }
    }
    else if (strstr(node->nombre, "Function"))
    {
        Function *func = (Function *)node;
        if (func->params)
        {
            Param *current = func->params;
            while (current)
            {
                print_dot_nodes((NodoBase *)current, current_id);
                current = current->next;
            }
        }
    }
    else if (strstr(node->nombre, "Declaration"))
    {
        Declaration *decl = (Declaration *)node;
        if (decl->expr)
        {
            print_dot_nodes(decl->expr, current_id);
        }
    }
    /*else if (strstr(node->nombre, "CallFunc"))
    {
        CallFunc *call = (CallFunc *)node;
        if (call->args)
        {
            ArgList *arg = call->args;
            while (arg)
            {
                if (arg->exp)
                    print_dot_nodes(arg->exp, current_id);
                arg = arg->next;
            }
        }
    }*/
}

void generate_ast_graphviz(NodoBase *root)
{
    if (!root)
        return;
    printf("\n");
    printf("digraph AST {\n");
    printf("rankdir=TB;\n");

    printf("node [shape=box, style=filled, color=lightgrey, fontname=\"Arial\"];\n");
    printf("edge [fontname=\"Arial\"];\n");

    nodo_counter = 0;
    print_dot_nodes(root, -1);
    printf("}\n");
}

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

    generate_ast_graphviz(g_root);

    /* liberar nodos (cada uno tiene el metodo para destruirse) */
    if (g_root)
        NodoBase_Destruir(g_root);
    Env_free(&global);

    return 0;
}
