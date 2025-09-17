%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "generador/generador.h"
  
  extern int yylex(void);
  void yyerror(const char *s);
%}

%union {
  double     num;
  char*      str;
  char   temp[32];    // Para guardar temporales
  char       ch;
  int        b;
}

%token <num> NUMBER
%token <str> STRING ID
%token <ch> CHAR
%token  INTEGER FLOAT TP_STRING BOOLEAN TP_CHAR
%token <b> TRUE FALSE
%token PRINT IF

%left '+' '-'
%left '*' '/'
%right UMINUS

%type <temp> expr

%start program

%%

program
  : lines
  ;

lines
  : /* vacío */
  | lines line
  ;

line
  : expr ';'                    { /* La expresión ya generó su código */ }
  | PRINT '(' expr ')' ';'      { addPrint($3); }
  ;

/* Esquema de traducción para expresiones */
expr
  : expr '+' expr {
      // Generar nuevo temporal para el resultado
      char* temp_resultado = newTemp();
      
      // Generar instrucción de suma
      addExpression(temp_resultado, $1, "+", $3);
      
      // Sintetizar el resultado
      strcpy($$, temp_resultado);
  }
  
  | expr '-' expr {
      char* temp_resultado = newTemp();
      addExpression(temp_resultado, $1, "-", $3);
      strcpy($$, temp_resultado);
  }
  
  | expr '*' expr {
      char* temp_resultado = newTemp();
      addExpression(temp_resultado, $1, "*", $3);
      strcpy($$, temp_resultado);
  }
  
  | expr '/' expr {
      char* temp_resultado = newTemp();
      addExpression(temp_resultado, $1, "/", $3);
      strcpy($$, temp_resultado);
  }
  
  | '-' expr %prec UMINUS {
      char* temp_resultado = newTemp();
      addExpression(temp_resultado, "0", "-", $2);  // 0 - expr
      strcpy($$, temp_resultado);
  }
  
  | '(' expr ')' {
      strcpy($$, $2);  // Solo pasar el temporal
  }
  
  | NUMBER {
      // Generar temporal y cargar el número
      char* temp = newTemp();
      char num_str[32];
      
      // Convertir número a string
      if ((int)$1 == $1) {
          snprintf(num_str, sizeof(num_str), "%d", (int)$1);
      } else {
          snprintf(num_str, sizeof(num_str), "%.2f", $1);
      }
      
      // Generar asignación: t1 = 42
      addAssign(temp, num_str);
      strcpy($$, temp);  // Devolver el temporal
  }
  
  | ID {
      // Generar temporal y cargar la variable
      char* temp = newTemp();
      addAssign(temp, $1);  // t1 = x
      strcpy($$, temp);  // Devolver el temporal
  }
  
  | TRUE {
      char* temp = newTemp();
      addAssign(temp, "1");
      strcpy($$, temp);  // Devolver el temporal
  }
  
  | FALSE {
      char* temp = newTemp();
      addAssign(temp, "0");
      strcpy($$, temp);  // Devolver el temporal
  }
  ;

%%

void yyerror(const char *s) {
  fprintf(stderr, "Error: %s\n", s);
}
