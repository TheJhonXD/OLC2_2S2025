%{
  #include <stdio.h>
  #include <stdlib.h>

  #include "ast/AbstractTerminal.h"
  #include "ast/expresiones/primitivos.h"
  #include "ast/expresiones/operacion.h"
  #include "ast/sentencias/print.h"
  #include "ast/sentencias/declaration.h"
  #include "ast/sentencias/assigment.h"
  #include "ast/expresiones/variable.h"


  extern int yylex(void);
  void yyerror(const char *s);

  /* Lista global de instrucciones */
  NodoBase** g_code = NULL;
  int        g_code_len = 0;
  int        g_code_cap = 0;

  static void code_push(NodoBase* n){
    if(!n) return;
    if(g_code_len == g_code_cap){
      g_code_cap = g_code_cap ? g_code_cap*2 : 16;
      g_code = (NodoBase**)realloc(g_code, g_code_cap * sizeof(NodoBase*));
    }
    g_code[g_code_len++] = n;
  }
%}

%define parse.error verbose
%locations

%union {
  double     num;
  NodoBase*  node;
  char*      str;
  int tipo;
}

%token <num> NUMBER
%token <str> STRING, ID
%token  INTEGER, FLOAT, TP_STRING, BOOLEAN
%token PRINT

%left '+' '-'
%left '*' '/'
%right UMINUS

%type <node> program lines line expr, declaration, assigment
%type <tipo> tipo
%start program

%%

program
  : lines                { /* code en g_code[] */ }
  ;

lines
  : /* vacío */
  | lines line
  | line
  ;

line
  : expr end                 { code_push($1); }
  | PRINT '(' expr ')' end   { code_push((NodoBase*)NewPrint(@1.first_line,@1.first_column,$3)); }
  | declaration end          { code_push($1); }
  | assigment end            { code_push($1); }
  ;

end
  : ';'
  ;

declaration
  : tipo ID '=' expr {
    $$ = (NodoBase*)NewDeclaration(@1.first_line,@1.first_column,$2,$1,$4);
  }
  ;

assigment
  : ID '=' expr {
    $$ = (NodoBase*)NewAssigment(@1.first_line,@1.first_column,$1,$3);
  }

tipo
  : INTEGER   { $$ = T_INTEGER; }
  | FLOAT    { $$ = T_FLOAT; }
  | TP_STRING { $$ = T_STRING; }
  | BOOLEAN  { $$ = T_BOOLEAN; }
  ;

expr
  : expr '+' expr            { $$ = (NodoBase*)NewOperation(@2.first_line,@2.first_column,$1,"+",$3); }
  | expr '-' expr            { $$ = (NodoBase*)NewOperation(@2.first_line,@2.first_column,$1,"-",$3); }
  | expr '*' expr            { $$ = (NodoBase*)NewOperation(@2.first_line,@2.first_column,$1,"*",$3); }
  | expr '/' expr            { $$ = (NodoBase*)NewOperation(@2.first_line,@2.first_column,$1,"/",$3); }
  | '-' expr %prec UMINUS    { $$ = (NodoBase*)NewOperation(@1.first_line,@1.first_column, (NodoBase*)NewPrimitive(@2.first_line,@2.first_column, SymInt(@2.first_line,@2.first_column,0)), "-", $2); }
  | '(' expr ')'             { $$ = $2; }
  | NUMBER {
    int i = (int)$1;
    if ((double)i == $1) $$ = (NodoBase*)NewPrimitive(@1.first_line,@1.first_column, SymInt(@1.first_line,@1.first_column, i));
    else  $$ = (NodoBase*)NewPrimitive(@1.first_line,@1.first_column, SymFloat(@1.first_line,@1.first_column, $1));
  }
  | STRING {
    $$ = (NodoBase*)NewPrimitive(@1.first_line,@1.first_column, SymStr(@1.first_line,@1.first_column,$1));
  }
  | ID {
    $$ = (NodoBase*)NewVariable(@1.first_line,@1.first_column,$1);
  }
  ;

%%
void yyerror(const char *s) {
  fprintf(stderr, "Error de sintaxis: %s\n", s);
}
