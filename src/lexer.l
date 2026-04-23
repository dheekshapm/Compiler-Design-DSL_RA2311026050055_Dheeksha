%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line_num = 1;
%}

%%

[ \t]+          { /* skip whitespace */ }
\n              { line_num++; }
"//".*          { /* skip single-line comments */ }

"int"           { return INT; }
"float"         { return FLOAT_TYPE; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"for"           { return FOR; }
"print"         { return PRINT; }
"return"        { return RETURN; }

[0-9]+\.[0-9]+ { yylval.fval = atof(yytext); return FLOAT_NUM; }
[0-9]+          { yylval.ival = atoi(yytext); return INT_NUM; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return ID; }

"=="            { return EQ; }
"!="            { return NEQ; }
"<="            { return LE; }
">="            { return GE; }
"&&"            { return AND; }
"||"            { return OR; }
"!"             { return NOT; }
"<"             { return '<'; }
">"             { return '>'; }
"="             { return '='; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
";"             { return ';'; }
"("             { return '('; }
")"             { return ')'; }
"{"             { return '{'; }
"}"             { return '}'; }

.               { fprintf(stderr, "Unknown character '%s' at line %d\n", yytext, line_num); }

%%

int yywrap() { return 1; }
