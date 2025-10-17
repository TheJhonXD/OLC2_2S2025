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
  #include "ast/sentencias/if.h"
  #include "ast/sentencias/block.h"
  #include "ast/sentencias/params.h"
  #include "ast/sentencias/function.h"
  #include "ast/expresiones/callFunc.h"
  #include "ast/expresiones/args.h"
  #include "ast/sentencias/return.h"
  #include "ast/expresiones/stringValueOf.h"


  extern int yylex(void);
  void yyerror(const char *s);

  /* Lista global de instrucciones */
  NodoBase* g_root = NULL;
  /*int        g_code_len = 0;
  int        g_code_cap = 0;

  static void code_push(NodoBase* n){
    if(!n) return;
    if(g_code_len == g_code_cap){
      g_code_cap = g_code_cap ? g_code_cap*2 : 16;
      g_code = (NodoBase**)realloc(g_code, g_code_cap * sizeof(NodoBase*));
    }
    g_code[g_code_len++] = n;
  }*/
%}

%define parse.error verbose
%locations

%union {
  double     num;
  NodoBase*  node;
  struct ArgList* args;  /* lista de argumentos */
  struct Param*   plist; /* lista de parámetros */
  char*      str;
  char       ch;
  int       b;
  int tipo;
}

%token <num> NUMBER
%token <str> STRING ID
%token <ch> CHAR
%token <b> TRUE FALSE
%token  INTEGER FLOAT TP_STRING BOOLEAN TP_CHAR
%token PRINT IF FUNC RETURN STRING_VALUEOF

%left '>' '<'
%left '+' '-'
%left '*' '/'
%right UMINUS

%type <node> program lines line expr declaration assigment block function callFunction
%type <tipo> tipo
%type <plist> params paramlist
%type <args> args arglist
%start program

%%

program
  : lines { g_root = $1; $$ = $1; }
  ;

lines
  : /* vacío */ { $$ = (NodoBase*)NewBlock(@$.first_line, @$.first_column); }
  | lines line  { addBlockStmt((Block*)$1, $2); $$ = $1; }
  ;

line
  : expr end                 { $$ = $1; }
  | PRINT '(' expr ')' end   { $$ = (NodoBase*)NewPrint(@1.first_line,@1.first_column,$3); }
  | declaration end          { $$ = $1; }
  | assigment end            { $$ = $1; }
  | block                    { $$ = $1; }
  | IF '(' expr ')' block    { $$ = (NodoBase*)NewIf(@1.first_line,@1.first_column,$3,$5); }
  | function                 { $$ = $1; }
  | RETURN expr end          { $$ = (NodoBase*)NewReturn(@1.first_line,@1.first_column,$2); }
  | RETURN end               { $$ = (NodoBase*)NewReturn(@1.first_line,@1.first_column,NULL); }
  ;

block 
  : '{' lines '}' {$$ = $2;}
  ;

end
  : ';'
  ;

function
  : FUNC tipo ID '(' params ')' block {
    $$ = (NodoBase*)NewFunction(@1.first_line, @1.first_column, $3, $2, (Param*)$5, $7);
  }
  ;

params
  : /* vacío */ { $$ = NULL; }
  | paramlist { $$ = $1; }
  ;

paramlist
  : tipo ID {
    Param *p = NewParam(@1.first_line, @1.first_column, $2, $1);
    $$ = p;
  }
  | tipo ID ',' paramlist {
    Param *p = NewParam(@1.first_line, @1.first_column, $2, $1);
    p->next = $4;
    $$ = p;
  }
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
  | TP_CHAR    { $$ = T_CHAR; }
  ;

expr
  : expr '+' expr            { $$ = (NodoBase*)NewOperation(@2.first_line,@2.first_column,$1,"+",$3); }
  | expr '-' expr            { $$ = (NodoBase*)NewOperation(@2.first_line,@2.first_column,$1,"-",$3); }
  | expr '*' expr            { $$ = (NodoBase*)NewOperation(@2.first_line,@2.first_column,$1,"*",$3); }
  | expr '/' expr            { $$ = (NodoBase*)NewOperation(@2.first_line,@2.first_column,$1,"/",$3); }
  | '-' expr %prec UMINUS    { 
    $$ = (NodoBase*)NewOperation(@1.first_line,@1.first_column,$2, "unario", NULL); 
  }
  | '(' expr ')'             { $$ = $2; }
  | expr '>' expr            { $$ = (NodoBase*)NewOperation(@2.first_line,@2.first_column,$1,">",$3); }
  | expr '<' expr            { $$ = (NodoBase*)NewOperation(@2.first_line,@2.first_column,$1,"<",$3); }
  | callFunction             { $$ = $1; }
  | NUMBER {
    int i = (int)$1;
    if ((double)i == $1) $$ = (NodoBase*)NewPrimitive(@1.first_line,@1.first_column, SymInt(@1.first_line,@1.first_column, i));
    else  $$ = (NodoBase*)NewPrimitive(@1.first_line,@1.first_column, SymFloat(@1.first_line,@1.first_column, $1));
  }
  | STRING {
    $$ = (NodoBase*)NewPrimitive(@1.first_line,@1.first_column, SymStr(@1.first_line,@1.first_column,$1));
  }
  | CHAR {
    $$ = (NodoBase*)NewPrimitive(@1.first_line,@1.first_column, SymChar(@1.first_line,@1.first_column,$1));
  }
  | ID {
    $$ = (NodoBase*)NewVariable(@1.first_line,@1.first_column,$1);
  }
  | TRUE {
    $$ = (NodoBase*)NewPrimitive(@1.first_line,@1.first_column, SymBool(@1.first_line,@1.first_column,1));
  }
  | FALSE {
    $$ = (NodoBase*)NewPrimitive(@1.first_line,@1.first_column, SymBool(@1.first_line,@1.first_column,0));
  }
  | STRING_VALUEOF '(' expr ')' {
    $$ = (NodoBase*)NewStringValueOf(@1.first_line,@1.first_column,$3);
  }
  ;

callFunction
  : ID '(' args ')' {
    $$ = (NodoBase*)NewCallFunc(@1.first_line, @1.first_column, $1, $3);
  }
  ;

args
  : /* vacío */ { $$ = NULL; }
  | arglist     { $$ = $1; }
  ;

arglist
  : expr             { $$ = NewArgList($1, NULL); }
  | expr ',' arglist { $$ = NewArgList($1, $3); }
  ;

%%
void yyerror(const char *s) {
  fprintf(stderr, "Error de sintaxis: %s\n", s);
}
