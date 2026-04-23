%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symtable.h"
#include "codegen.h"

extern int yylex();
extern int line_num;
void yyerror(const char *s);

ASTNode *root = NULL;
%}

%union {
    int   ival;
    float fval;
    char *sval;
    struct ASTNode *node;
}

%token <ival> INT_NUM
%token <fval> FLOAT_NUM
%token <sval> ID
%token INT FLOAT_TYPE IF ELSE WHILE FOR PRINT RETURN
%token EQ NEQ LE GE AND OR NOT

%type <node> program stmtlist stmt expr term factor condition block
%type <node> decl assign ifstmt whilestmt forstmt printstmt

%right '='
%left OR
%left AND
%left EQ NEQ
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/'
%right NOT UMINUS

%%

program
    : stmtlist  { root = $1; }
    ;

stmtlist
    : stmtlist stmt  { $$ = make_node(NODE_STMTLIST, $1, $2); }
    | stmt           { $$ = $1; }
    ;

stmt
    : decl       { $$ = $1; }
    | assign ';' { $$ = $1; }
    | ifstmt     { $$ = $1; }
    | whilestmt  { $$ = $1; }
    | forstmt    { $$ = $1; }
    | printstmt  { $$ = $1; }
    | RETURN expr ';' { $$ = make_node(NODE_RETURN, $2, NULL); }
    ;

decl
    : INT ID ';'        { $$ = make_decl_node($2, TYPE_INT, NULL); }
    | INT ID '=' expr ';' { $$ = make_decl_node($2, TYPE_INT, $4); }
    | FLOAT_TYPE ID ';'   { $$ = make_decl_node($2, TYPE_FLOAT, NULL); }
    | FLOAT_TYPE ID '=' expr ';' { $$ = make_decl_node($2, TYPE_FLOAT, $4); }
    ;

assign
    : ID '=' expr  { $$ = make_assign_node($1, $3); }
    ;

ifstmt
    : IF '(' condition ')' block           { $$ = make_node(NODE_IF, $3, $5); }
    | IF '(' condition ')' block ELSE block { $$ = make_ifelse_node($3, $5, $7); }
    ;

whilestmt
    : WHILE '(' condition ')' block { $$ = make_node(NODE_WHILE, $3, $5); }
    ;

forstmt
    : FOR '(' assign ';' condition ';' assign ')' block
        { $$ = make_for_node($3, $5, $7, $9); }
    ;

printstmt
    : PRINT '(' expr ')' ';' { $$ = make_node(NODE_PRINT, $3, NULL); }
    ;

block
    : '{' stmtlist '}' { $$ = $2; }
    | '{' '}'          { $$ = NULL; }
    ;

condition
    : expr EQ  expr { $$ = make_node(NODE_EQ,  $1, $3); }
    | expr NEQ expr { $$ = make_node(NODE_NEQ, $1, $3); }
    | expr '<'  expr { $$ = make_node(NODE_LT,  $1, $3); }
    | expr '>'  expr { $$ = make_node(NODE_GT,  $1, $3); }
    | expr LE  expr { $$ = make_node(NODE_LE,  $1, $3); }
    | expr GE  expr { $$ = make_node(NODE_GE,  $1, $3); }
    | condition AND condition { $$ = make_node(NODE_AND, $1, $3); }
    | condition OR  condition { $$ = make_node(NODE_OR,  $1, $3); }
    | NOT condition           { $$ = make_node(NODE_NOT, $2, NULL); }
    ;

expr
    : expr '+' term { $$ = make_node(NODE_ADD, $1, $3); }
    | expr '-' term { $$ = make_node(NODE_SUB, $1, $3); }
    | term          { $$ = $1; }
    ;

term
    : term '*' factor { $$ = make_node(NODE_MUL, $1, $3); }
    | term '/' factor { $$ = make_node(NODE_DIV, $1, $3); }
    | factor          { $$ = $1; }
    ;

factor
    : INT_NUM    { $$ = make_int_node($1); }
    | FLOAT_NUM  { $$ = make_float_node($1); }
    | ID         { $$ = make_id_node($1); }
    | '(' expr ')' { $$ = $2; }
    | '-' factor %prec UMINUS { $$ = make_node(NODE_NEG, $2, NULL); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", line_num, s);
}
