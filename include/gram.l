%{
#include <stdio.h>
#include <token.hpp>
%}

%option c++
%option noyywrap
%option yylineno

%%

"+"    { return PLUS; }
"*"    { return MULTIPLY; }
"("    { return LPAREN;}
")"    { return RPAREN; }

[0-9]+  { return NUMBER; }

[a-zA-Z][a-zA-Z0-9]* { return IDENTIFIER; }

[ \t]  { /* ignore */ }
\n     { /* ignore */ }
.      { return ERROR; }

%%