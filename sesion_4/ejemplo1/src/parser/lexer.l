%option noyywrap nodefault noinput nounput
%{
  #include <stdlib.h>
  typedef struct NodoBase NodoBase;
  #include "parser.tab.h"
%}

DIG   [0-9]
NUM   {DIG}+(\.{DIG}+)?

%%
print                 { return PRINT; }
{NUM}                 { yylval.num = atof(yytext); return NUMBER; }

[ \t\r]+              { /* skip */ }
\n                    { return '\n'; }
[;]                   { return ';'; }

[(]                   { return '('; }
[)]                   { return ')'; }
[+]                   { return '+'; }
[-]                   { return '-'; }
[*]                   { return '*'; }
[/]                   { return '/'; }

.                     { /* ignora otros */ }
%%
