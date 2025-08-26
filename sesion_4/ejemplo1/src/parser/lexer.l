%option noyywrap nodefault noinput nounput
%{
  #include <stdlib.h>
  typedef struct NodoBase NodoBase;
  #include "parser.tab.h"
%}
TRU      "true"
FAL      "false"
KW_PRINT "print"
KW_IF    "if"
KW_INT   "int"
KW_FLOAT "float"
KW_STR   "string"
KW_CHAR  "char"
KW_BOOL     "boolean"
DIG   [0-9]
NUM   {DIG}+(\.{DIG}+)?
STR   \"([^\\\n]|\\.)*\"
CH  \'([^\\\n]|\\.)\'
ID [A-Za-z_][A-Za-z0-9_]*
WS  [ \t\n\r]+


%%

{KW_PRINT}        { return PRINT; }
{KW_IF}           { return IF; }
{KW_INT}          { return INTEGER; }
{KW_FLOAT}        { return FLOAT; }
{KW_STR}          { return TP_STRING; }
{KW_BOOL}            { return BOOLEAN; }
{KW_CHAR}         { return TP_CHAR; }
{TRU}             { return TRUE; }
{FAL}             { return FALSE; }

{NUM}                 { yylval.num = atof(yytext); return NUMBER; }
{STR}                 { yylval.str = strdup(yytext + 1); yylval.str[strlen(yylval.str) - 1] = '\0'; return STRING; }
{CH}                  { yylval.ch = yytext[1]; return CHAR; }

{ID}                  { yylval.str = strdup(yytext); return ID; }

{WS}                          { /* skip whitespace */ }
"//".*                        { /* skip comentario de una linea */ }
"/*"([^*]|\*+[^*/])*\*+"/"    { /* skip comentario multi-linea */ }
[;]                   { return ';'; }

[(]                   { return '('; }
[)]                   { return ')'; }
[{]                   { return '{'; }
[}]                   { return '}'; }
[+]                   { return '+'; }
[-]                   { return '-'; }
[*]                   { return '*'; }
[/]                   { return '/'; }
["="]                 { return '='; }
[">"]                 { return '>'; }
["<"]                 { return '<'; }

.                     { /* ignora otros */ }
%%
