%option noyywrap nodefault noinput nounput
%{
  #include <stdlib.h>
  typedef struct NodoBase NodoBase;
  #include "parser.tab.h"
%}

DIG   [0-9]
NUM   {DIG}+(\.{DIG}+)?
STR   \"([^\\\n]|\\.)*\"
CH  \'([^\\\n]|\\.)\'
ID [A-Za-z_][A-Za-z0-9_]*

%%
print                 { return PRINT; }
{NUM}                 { yylval.num = atof(yytext); return NUMBER; }
{STR}                 { yylval.str = strdup(yytext + 1); yylval.str[strlen(yylval.str) - 1] = '\0'; return STRING; }
{CH}                  { yylval.ch = yytext[1]; return CHAR; }
int                     { return INTEGER; }
float                   { return FLOAT; }
string                  { return TP_STRING; }
boolean                 { return BOOLEAN; }
char                    { return TP_CHAR; }

{ID}                  { yylval.str = strdup(yytext); return ID; }

[ \t]+                { /* skip */ }

[ \t\r\n]+              { /* skip */ }
[;]                   { return ';'; }

[(]                   { return '('; }
[)]                   { return ')'; }
[+]                   { return '+'; }
[-]                   { return '-'; }
[*]                   { return '*'; }
[/]                   { return '/'; }
["="]                 { return '='; }

.                     { /* ignora otros */ }
%%
