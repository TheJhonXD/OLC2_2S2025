%option noyywrap nodefault noinput nounput
%{
  #include <stdlib.h>
  typedef struct NodoBase NodoBase;
  #include "parser.tab.h"
%}

DIG   [0-9]
NUM   {DIG}+(\.{DIG}+)?
STR   \"([^\\\n]|\\.)*\"

%%
print                 { return PRINT; }
{NUM}                 { yylval.num = atof(yytext); return NUMBER; }
{STR}                 { yylval.str = strdup(yytext + 1); yylval.str[strlen(yylval.str) - 1] = '\0'; return STRING; }

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
